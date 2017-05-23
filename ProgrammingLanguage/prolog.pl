mother(X, Y) :- female(X), parent(X, Y).

father(X, Y) :- male(X), parent(X, Y).

child(X, Y) :- parent(Y, X).

sibling(X, Y) :- parent(Z, X), parent(Z, Y).

sister(X, Y) :- female(X), female(Y), sibling(X, Y).

brother(X, Y) :- male(X), male(Y), sibling(X, Y).

daughter(X, Y) :- female(X), parent(Y, X).

son(X, Y) :- male(X), parent(Y, X).

uncle(X, Y) :- male(X), cousin(Y, X).

aunt(X, Y) :- female(X), cousin(Y, X).

cousin(X, Y) :- parent(Z, X), sibling(Z, Y).

% Do not modify the following facts.  Complete the predicates 
% above so that they work with the following facts.

male(mark).
male(mel).
male(richard).
male(tom).
male(adam).

female(amy).
female(jane).
female(joan).
female(betty).
female(rosa).
female(fran).


parent(mel, joan).
parent(jane, betty).
parent(jane, tom).
parent(richard, adam).
parent(richard, rosa).
parent(joan, fran).
parent(mark, jane).
parent(mark, richard).
parent(amy, jane).
parent(amy, richard).
parent(amy, joan).
