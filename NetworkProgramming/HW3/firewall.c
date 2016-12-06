/* Demonstration program of reading packet trace files recorded by pcap
 * (used by tshark and tcpdump) and dumping out some corresponding information
 * in a human-readable form.
 *
 * Note, this program is limited to processing trace files that contains
 * UDP packets.  It prints the timestamp, source port, destination port,
 * and length of each such packet.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <net/if.h>

#include <pcap.h>


/* We've included the UDP header struct for your ease of customization.
 * For your protocol, you might want to look at netinet/tcp.h for hints
 * on how to deal with single bits or fields that are smaller than a byte
 * in length.
 *
 * Per RFC 768, September, 1981.
 */
struct UDP_hdr {
    uint16_t    uh_sport;        /* source port */
    uint16_t    uh_dport;        /* destination port */
    uint16_t    uh_ulen;        /* datagram length */
    uint16_t    uh_sum;            /* datagram checksum */
};

struct packet_meta {
    uint32_t ipv4_src;
    uint32_t ipv4_dst;
    uint16_t src_port;
    uint16_t dst_port;
//    uint8_t  protocol;
};

int rules_count = 0;
struct packet_meta *blacklist = NULL;


/* Some helper functions, which we define at the end of this file. */

/* Returns a string representation of a timestamp. */
const char *timestamp_string(struct timeval ts);

/* Report a problem with dumping the packet with the given timestamp. */
void problem_pkt(struct timeval ts, const char *reason);

/* Report the specific problem of a packet being too short. */
void too_short(struct timeval ts, const char *truncated_hdr);

int get_packet_meta(const unsigned char *packet, struct timeval ts, unsigned int capture_len,
                    struct packet_meta *out)
{
    /* For simplicity, we assume Ethernet encapsulation. */
    if (capture_len < sizeof(struct ether_header))
    {
        /* We didn't even capture a full Ethernet header, so we
         * can't analyze this any further.
         */
        too_short(ts, "Ethernet header");
        return 0;
    }

    /* Skip over the Ethernet header. */
    packet += sizeof(struct ether_header);
    capture_len -= sizeof(struct ether_header);

    if (capture_len < sizeof(struct ip))
    { /* Didn't capture a full IP header */
        too_short(ts, "IP header");
        return 0;
    }

    struct ip *ip = (struct ip*) packet;
    unsigned int IP_header_length = ip->ip_hl * 4;    /* ip_hl is in 4-byte words */

    if (capture_len < IP_header_length)
    { /* didn't capture the full IP header including options */
        too_short(ts, "IP header with options");
        return 0;
    }

    out->ipv4_src = ip->ip_src.s_addr;
    out->ipv4_dst = ip->ip_src.s_addr;

    /* Skip over the IP header to get to next layer. */
    packet += IP_header_length;
    capture_len -= IP_header_length;

    if (ip->ip_p == IPPROTO_TCP)
    {
//        out->protocol = IPPROTO_TCP;

        if (capture_len < sizeof(struct tcphdr))
        {
            too_short(ts, "TCP header");
            return 0;
        }

        struct tcphdr *tcp = (struct tcphdr *) packet;
        out->dst_port = ntohs(tcp->th_dport);
        out->src_port = ntohs(tcp->th_sport);
        return 1;
    }
    else if (ip->ip_p == IPPROTO_UDP)
    {
//        out->protocol = IPPROTO_UDP;

        if (capture_len < sizeof(struct UDP_hdr))
        {
            too_short(ts, "UDP header");
            return 0;
        }

        struct UDP_hdr *udp = (struct UDP_hdr*) packet;
        out->dst_port = ntohs(udp->uh_dport);
        out->src_port = ntohs(udp->uh_sport);
        return 1;
    }
    else
    {
        puts("not UDP nor TCP, skip unknown protocol");
        return 0;
    }
}

int packet_filter(const unsigned char *packet, struct timeval ts, unsigned int capture_len)
{
    struct packet_meta meta;
    if (get_packet_meta(packet, ts, capture_len, &meta) == 0) {
        return 0;
    }

    char ipv4_dst[INET_ADDRSTRLEN], ipv4_src[INET_ADDRSTRLEN];

    if (inet_ntop(AF_INET, &(meta.ipv4_src), ipv4_src, INET_ADDRSTRLEN) == NULL ||
        inet_ntop(AF_INET, &(meta.ipv4_dst), ipv4_dst, INET_ADDRSTRLEN) == NULL)
    {
        puts("Can not convert ip to string");
        return 0;
    }

    int status = 1;
    for (int i = 0; i < rules_count && status; i++) {
        if (memcmp(&meta, &blacklist[i], sizeof(struct packet_meta)) == 0) {
            status = 0;
        }
    }

    printf("Got packet from %s:%d to %s:%d, status: %s\n",
            ipv4_src, meta.src_port, ipv4_dst, meta.dst_port, status ? "accept" : "reject");

    return status;
}

void parse_rule_file(const char * const file)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Can not read rule file\n");
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t readed;
    while ((readed = getline(&line, &len, fp)) != -1)
    {
        if (line[readed - 1] == '\n') line[readed - 1] = 0;
        char ipv4_src[INET_ADDRSTRLEN], ipv4_dst[INET_ADDRSTRLEN];
        uint16_t src_port, dst_port;
        if (sscanf(line, "%15s %15s %hu %hu", ipv4_src, ipv4_dst, &src_port, &dst_port) != 4)
        {
            goto err;
        } else {
            struct packet_meta rule;

            if (inet_pton(AF_INET, ipv4_src, &rule.ipv4_src) == -1 ||
                inet_pton(AF_INET, ipv4_dst, &rule.ipv4_dst) == -1) {
                goto err;
            }

            rule.src_port = src_port;
            rule.dst_port = dst_port;

            blacklist = realloc(blacklist, sizeof(struct packet_meta) * ++rules_count);
            blacklist[rules_count - 1] = rule;
        }
        continue;

err:
        fprintf(stderr, "Can not parse this line: \"%s\"\n", line);
    }
    free(line);

    fclose(fp);
    printf("Parsed rules: %d\n", rules_count);
}

int main(int argc, char *argv[])
{
    pcap_t *pcap;
    const unsigned char *packet;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr header;
    int pkt_count = 0;

    pcap_t *adhandle;
    pcap_dumper_t *dumpfile;

    /* We expect exactly one argument, the name of the file to dump. */
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s rule.txt in.pcap out.pcap\n", argv[0]);
        exit(1);
    }

    parse_rule_file(argv[1]); // success or die

    pcap = pcap_open_offline(argv[2], errbuf);
    if (pcap == NULL)
    {
        fprintf(stderr, "error reading pcap file: %s\n", errbuf);
        exit(1);
    }

    dumpfile = pcap_dump_open(pcap, argv[3]);

    if(dumpfile==NULL)
    {
        fprintf(stderr,"\nError opening output file\n");
        return -1;
    }

    /* Now just loop through extracting packets as long as we have
     * some to read.
     */
    while ((packet = pcap_next(pcap, &header)) != NULL){
        int res = packet_filter(packet, header.ts, header.caplen);
        // printf("Processing %d packet, result: %s\n", pkt_count, res ? "accept" : "reject");
        pkt_count++;
        if (res)
            pcap_dump((unsigned char *) dumpfile, &header, packet);
    }

    // terminate
    return 0;
}


/* Note, this routine returns a pointer into a static buffer, and
 * so each call overwrites the value returned by the previous call.
 */
const char *timestamp_string(struct timeval ts)
{
    static char timestamp_string_buf[256];

    sprintf(timestamp_string_buf, "%d.%06d",
            (int) ts.tv_sec, (int) ts.tv_usec);

    return timestamp_string_buf;
}

void problem_pkt(struct timeval ts, const char *reason)
{
    fprintf(stderr, "%s: %s\n", timestamp_string(ts), reason);
}

void too_short(struct timeval ts, const char *truncated_hdr)
{
    fprintf(stderr, "packet with timestamp %s is truncated and lacks a full %s\n",
            timestamp_string(ts), truncated_hdr);
}
