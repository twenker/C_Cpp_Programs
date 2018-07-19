/* Facts */
route(phoenix, flagstaff, 144).
route(phoenix, tucson, 111).
route(phoenix, lax, 372).
route(phoenix, sandiego, 355).
route(phoenix, dallas, 1064).
route(lax, sandiego, 120).
route(lax, sanfrancisco, 386).
route(dallas, springfield, 416).
route(springfield, chicago, 511).

/* Rules */
connected(X, Y) :-
        route(X, Y, _);
        (route(X, Z, _), route(Z, Y, _));
        (route(X, W, _), route(W, Z, _), route(Z, Y, _)).

distance(X, Y, Dist) :-
        (route(X, Y, C), Dist is C,
	write('Dist = '), write(Dist));

        (route(X, Z, C1), route(Z, Y, C2), Dist is C1+C2,
        write('Dist = '), write(Dist));

        (route(X, W, C1), route(W, Z, C2), route(Z, Y, C3), Dist is
        C1+C2+C3,
        write('Dist = '), write(Dist)).









