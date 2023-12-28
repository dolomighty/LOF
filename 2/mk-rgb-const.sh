#!/bin/bash
cat $1 \
| sed 's/rgb[(]/\nrgb /g' \
| awk '
match($0,/^rgb ([0-9]+)[^0-9]+([0-9]+)[^0-9]+([0-9]+)/,A){
    R[NR]=A[1]
    G[NR]=A[2]
    B[NR]=A[3]
#    print A[1] " " A[2] " " A[3]
}
END{
    printf("const auto R = std::vector<int>({ ")
    SEP=""
    for( N in R ){ printf( SEP R[N]); SEP = ", " }
    print " });"

    printf("const auto G = std::vector<int>({ ")
    SEP=""
    for( N in G ){ printf( SEP G[N]); SEP = ", " }
    print " });"

    printf("const auto B = std::vector<int>({ ")
    SEP=""
    for( N in B ){ printf( SEP B[N]); SEP = ", " }
    print " });"
}
'


exit
exit
exit
exit
exit
exit
exit

converte la scala rgb contenuta nell'svg
prodotto dal sito 
https://hihayk.github.io/scale/#4/4/70/69/10/47/14/45/EDE117/28/109/80/white
in tre scale RGB const int



