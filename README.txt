To run the program, run the following commands inside the "project" folder:


--------------------------
make
./app --help
--------------------------

Description of parameters:

Usage: app [OPTION...] DATASET FILE NAME ---> (It is mandatory to enter the name of the dataset)

  -b, --benchmark            Compute jaccard coef. between ranking
  -o, --filename-output=FILE Output dataset file name
  -?, --help                 Give this help list
      --usage                Give a short usage message


Commands example:

./app datasets/web-NotreDame.txt -o results/web-NotreDame-out.txt
./app datasets/web-NotreDame.txt -o results/web-NotreDame-out.txt -b


