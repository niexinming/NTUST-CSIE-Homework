import numpy as np
import csv

fp = open('./LargeTrain.csv')
reader = csv.reader(fp)

col = next(reader)
X = np.array([ list(map(float, row[:-1])) for row in reader ])

fp.seek(0)
reader = csv.reader(fp)
next(reader)
y = [ int(row[-1]) for row in reader ]

#print(col)
#print(X[:5])
#print(y[:5])

from sklearn.ensemble import ExtraTreesClassifier
clf = ExtraTreesClassifier(n_estimators=250, random_state=0)
clf = clf.fit(X, y)
imp = clf.feature_importances_

for v, i in sorted((v, i) for i, v in enumerate(imp))[-1:-20:-1]:
    print('%.4f\t%s' % (v, col[i]))

"""
0.0134	section_names_header
0.0111	ent_q_diffs_var
0.0080	ent_p_8
0.0070	ent_q_diff_diffs_2_min
0.0061	Img4
0.0060	ent_q_diffs_0
0.0059	dc_por
0.0057	GetEnvironmentStrings
0.0056	IsValidCodePage
0.0052	.text_por
0.0051	Img33
0.0050	Img7
0.0050	ent_q_diff_diffs_2_mean
0.0049	ent_q_diff_diffs_10
0.0049	ent_q_diff_diffs_2_median
0.0049	Img43
0.0046	FreeEnvironmentStringsW
0.0046	ent_p_18
0.0046	db3_rdata
"""
