% Analizador léxico simple en Prolog
% Busca las raíces "aud" y "regul" dentro de un texto latino.

% -----------------------------
% Alfabeto relevante
% -----------------------------

simbolo(a).
simbolo(e).
simbolo(d).
simbolo(g).
simbolo(l).
simbolo(r).
simbolo(u).

% -----------------------------
% Reconocimiento de patrones
% -----------------------------

patron_aud([a, u, d | _]).

patron_regul([r, e, g, u, l | _]).

% -----------------------------
% Buscar patrón dentro de una lista
% -----------------------------

contiene_patron(Texto) :-
    patron_aud(Texto).

contiene_patron(Texto) :-
    patron_regul(Texto).

contiene_patron([_ | Resto]) :-
    contiene_patron(Resto).

% -----------------------------
% Identificar qué patrón aparece
% -----------------------------

encontrar(aud, Texto) :-
    patron_aud(Texto).

encontrar(aud, [_ | Resto]) :-
    encontrar(aud, Resto).

encontrar(regul, Texto) :-
    patron_regul(Texto).

encontrar(regul, [_ | Resto]) :-
    encontrar(regul, Resto).
