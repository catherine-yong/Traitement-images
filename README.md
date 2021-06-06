# Traitement d'images en C

Dans le cadre de notre cursus à Polytech Sorbonne en MAIN3, nous avons pu aborder le sujet du Traitement d'images en C.

La problématique principale était : **" Comment un ordinateur peut-il différencier deux images ? "**

Pour cela, nous sommes partis des deux critères qui caractérisent une image : la répartition des pixels, avec le calcul de la **moyenne quadratique locale** (mse.c) entre deux images, et la **quantité de rouge, vert et bleu** dans chaque image (couleur.c) . Cependant, ces deux critères ne sont pas suffisants. En effet, deux images identiques mais dont l'une est floue, seront considérées comme étant différentes par l'algorithme de la moyenne quadratique locale. Par ailleurs, si quelques pixels parasites sont présents dans une des images, ceci faussera certainement le résultat.

C'est pourquoi il a fallu rechercher d'autres moyens d'étude.

Tout d'abord, **filtre de Sobel** nous permet de minimiser le problème rencontré avec l'image floue : l'image rendue par cet algorithme est seulement composée des délimitations de chacun des objets de l'image, là où le changement de couleur est le plus brusque.

Ensuite, l'**algorithme de lissage** nous permet de réduire la quantité de pixels parasites. En divisant l'image en un certain nombre de cases, on réalise ensuite la moyenne pour chacune de ses cases. Pour chaque pixel de cette case, s'il est trop éloigné de la moyenne locale, alors on le remplace par cette dernière donnée. (moyenne_local.c et Makefile)

En appliquant l'algorithme de la moyenne quadratique locale aux résultats des deux algorithmes, l'ordinateur est maintenant en mesure de mieux déterminer si deux images sont identiques ou non.

Nous avons utilisé la bibliothèque SDL2. C'est une bibliothèque complète et adaptée pour cette étude car souvent utilisée dans le domaine du traitement d'images. Nous pouvons également noter que ce projet aurait aussi été possible en Python ou Matlab.
