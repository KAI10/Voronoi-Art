printf "Compiling voronoi cpp ... "
g++ voronoi.cpp -o voronoi.out
printf "Compiled.\n"

printf "Generating site points from image ... "
python generate_site_points.py $1
printf "Generated.\n"

printf "Generating Voronoi Polygons ... "
./voronoi.out < site_points.txt
printf "Generated.\n"

printf "Drawing Voronoi Diagram ... \n"
python draw_voronoi.py