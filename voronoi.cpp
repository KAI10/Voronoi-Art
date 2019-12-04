#include <bits/stdc++.h>
#define LEFT -1
#define RIGHT 1
#define eps 1e-10
#define BORDER_DIS 4
using namespace std;

ofstream fout("test.out");

double sq(double val){
    return val*val;
}

struct Point{

    double x,y;
    Point(double _x = 0.0, double _y = 0.0){
        x = _x, y = _y;
    }

    double dist(Point p){

        return sqrt(sq(x-p.x)+sq(y-p.y));
    }

    void print(){
        cout<<"("<<x<<","<<y<<")";
    }
};

struct Edge{

    Point st, ed;
    double A,B,C; //Ax + By + C = 0
    bool isVertical;
    bool isSegment;

    Edge(Point a, Point b){
        isVertical = false;
        st = a;
        ed = b;

        double dx = st.x - ed.x;
        double dy = st.y - ed.y;

        A = -dy;
        B = dx;
        C = st.x*dy - st.y*dx;

        if(fabs(dx)<eps)isVertical = true;
        isSegment = true;
    }

    Edge(double _A, double _B, double _C){
        st = Point(0.0,0.0); ed = Point(0.0,0.0); //keeping unspecified
        isVertical = false;
        A = _A, B = _B, C = _C;
        if(fabs(B)<eps)isVertical = true;
        isSegment = false;
    }

    bool is_Degenerate(){

        if(fabs(st.x-ed.x)<eps && fabs(st.y-ed.y)<eps && fabs(A)<eps && fabs(B)<eps)return true;
        return false;
    }

    double Point_to_Line_dis(Point p){
        double num = fabs(A*p.x+B*p.y+C);
        double denum = sqrt(sq(A)+sq(B));

        return num/denum;
    }

    pair<bool, Point> Intersect(Edge l){

        //first check if the lines are parallel

        double val1 = A*l.B;
        double val2 = B*l.A;

        if(fabs(val1-val2)<eps){
            //cout<<"does not intersect because parallel\n";
             return make_pair(false,Point(0.0,0.0));
        }

        double hor = val2-val1;

        double xlob = C*l.B-B*l.C;
        double ylob = A*l.C-C*l.A;

        Point intersectionPoint(Point(xlob/hor,ylob/hor));

        if(!isSegment && !l.isSegment)return make_pair(true,intersectionPoint);

        else if(isSegment && !l.isSegment){
            double dx = ed.x-st.x;
            double dy = ed.y-st.y;
            //cout<<"intersects at";
            //intersectionPoint.print();

            double t=2.0;

            if(fabs(dy)<eps)
                t = (intersectionPoint.x-st.x)/dx;
            else
                t = (intersectionPoint.y-st.y)/dy;

            //cout<<t<<"\n";

            if((fabs(t)<eps || fabs(t-1.0)<eps) || (t>0.0 && t<1.0))return make_pair(true,intersectionPoint);
            else{
                //cout<<"does not intersect because out of segment bound\n";
                return make_pair(false,intersectionPoint);
            }
        }

        else if(!isSegment && l.isSegment){
            double dx = l.ed.x-l.st.x;
            double dy = l.ed.y-l.st.y;

            double t=2.0;

            if(fabs(dy)<eps)
                t = (intersectionPoint.x-l.st.x)/dx;
            else
                t = (intersectionPoint.y-l.st.y)/dy;

            if((fabs(t)<eps || fabs(t-1.0)<eps) || (t>0.0 && t<1.0))return make_pair(true,intersectionPoint);
            else{
                //cout<<"does not intersect because out of segment bound\n";
                return make_pair(false,intersectionPoint);
            }
        }

        //this is unreachable
        cout<<"unreachable code\n";
        return make_pair(false,Point(0.0,0.0));

    }

    Edge findPerpendicularBisector(){ //must be a segment

        double mx = (st.x+ed.x)/2.0;
        double my = (st.y+ed.y)/2.0;

        double newA = -B;
        double newB = A;
        double newC = -(newA*mx+newB*my);

        Edge perp(newA,newB,newC);

        return perp;
    }

    int getSide(Point p){

        double val = A*p.x + B*p.y + C;

        if(fabs(val)<eps)return 0;

        if(val<0.0)return LEFT;

        return RIGHT;

    }

    void clip(Point p, int keep_Start){
        if(keep_Start==1){
            ed = p;
        }
        else{
            st = p;
        }
    }

    void print(string name){
        if(isSegment){
            cout<<name<<" is a segment with endpoints ";
            st.print();
            cout<<" and ";
            ed.print();
            cout<<"\n";
        }
        else{
            cout<<name<<" is a line "<<A<<"x+"<<B<<"y+"<<C<<"\n";
        }
    }

};


struct Cell{

    Point site;
    vector<Edge>regionEdges;

    Cell(Point centroid){
        site = centroid;
    }

    void addEdge(Edge e){

        if(!e.is_Degenerate())regionEdges.push_back(e);
    }

    void print(int id){
        //ofstream fout("test.out");

        //fout<<"Printing cell info id: "<<id<<"\n";
        //fout<<"Site Point is: "<<site.x<<","<<site.y<<"\n";
        if(regionEdges.size() <= 0) return;


        fout << "[ ";
        for(int i = 0; i<regionEdges.size(); ++i){
            Edge e = regionEdges[i];
            Point p1 = e.st;
            Point p2 = e.ed;
            fout<< "(" << p1.x<<","<<p1.y<<"), ("<<p2.x<<","<<p2.y<<")";
            if (i == regionEdges.size() - 1) fout << "]";
            else fout << ", ";
        }

        fout << endl;
        //fout.close();
    }
    void print_console(int id){
        //ofstream fout("test.out");

        //fout<<"Printing cell info id: "<<id<<"\n";
        //fout<<"Site Point is: "<<site.x<<","<<site.y<<"\n";
        if(regionEdges.size() <= 0) return;


        cout << "[ ";
        for(int i = 0; i<regionEdges.size(); ++i){
            Edge e = regionEdges[i];
            Point p1 = e.st;
            Point p2 = e.ed;
            cout<< "(" << p1.x<<","<<p1.y<<"), ("<<p2.x<<","<<p2.y<<")";
            if (i == regionEdges.size() - 1) cout << "]";
            else cout << ", ";
        }

        cout << endl;
        //fout.close();
    }
};

vector<Point>S;
vector<Edge>E;
vector<Cell>C;

void takeSitePoints(int n){
    for(int i = 1; i<=n; ++i){

        double x,y;
        scanf("%lf %lf",&x,&y);

        S.push_back(Point(x,y));
    }
}

void add_boundary_Cells(double width, double height){
//[(width/2, -10*height), (-10*width, height/2)]
//[(-10*width, height/2), (width/2, height/2)].
    Cell c1(Point(-width,-height));
    c1.addEdge(Edge(Point(width/2.0, height/2.0),Point(width/2.0,-BORDER_DIS*height)));
    c1.addEdge(Edge(Point(width/2.0,-BORDER_DIS*height),Point(-BORDER_DIS*width,height/2.0)));
    c1.addEdge(Edge(Point(-BORDER_DIS*width,height/2.0),Point(width/2.0,height/2.0)));

    Cell c2(Point(2.0*width,-height));
    c2.addEdge(Edge(Point(width/2.0, height/2.0),Point(width/2.0,-BORDER_DIS*height)));
    c2.addEdge(Edge(Point(width/2.0,-BORDER_DIS*height),Point(BORDER_DIS*width,height/2.0)));
    c2.addEdge(Edge(Point(BORDER_DIS*width,height/2.0),Point(width/2.0,height/2.0)));

    Cell c3(Point(2.0*width,2.0*height));
    c3.addEdge(Edge(Point(BORDER_DIS*width,height/2.0),Point(width/2.0,height/2.0)));
    c3.addEdge(Edge(Point(width/2.0, height/2.0),Point(width/2.0,BORDER_DIS*height)));
    c3.addEdge(Edge(Point(width/2.0,BORDER_DIS*height),Point(BORDER_DIS*width,height/2.0)));

    Cell c4(Point(-width,2.0*height));
    c4.addEdge(Edge(Point(width/2.0, height/2.0),Point(width/2.0,BORDER_DIS*height)));
    c4.addEdge(Edge(Point(width/2.0,BORDER_DIS*height),Point(-BORDER_DIS*width,height/2.0)));
    c4.addEdge(Edge(Point(-BORDER_DIS*width,height/2.0),Point(width/2.0,height/2.0)));

    C.push_back(c1);
    C.push_back(c2);
    C.push_back(c3);
    C.push_back(c4);
}

bool isSameSide_not_Intersect(int cside, int startside, int endside){
    if(startside==0 && endside==0)return false;
    if(startside==0)return cside==endside;
    return cside==startside;
}

void process_new_site(Point site){
    cout<<"\n\n\n";
    Cell newcell(site); //line 4
    cout<<"checking diagram for new site ";
    site.print();
    cout<<"\n";
    int op = 0;
    for(int i = 0; i<C.size(); ++i){ //line 5
        Cell c = C[i];
        cout<<"\n\n";
        op++;
        Edge seg(c.site,site);
        seg.print("line_segment_connecting_two_sites");
        Edge pb = seg.findPerpendicularBisector();
        pb.print("perpendicular_bisector"); //line 6 -> upto this seems correct

        vector<Point>X; //line 7
        vector<int>toDelete;

        for(int i = 0; i<(int)c.regionEdges.size(); ++i){
            cout<<"\n";
            cout<<"Will test spatial relationship\n";
            Edge e = c.regionEdges[i];
            e.print("cell_edge_e");
            //each e is a segment
            pair<bool,Point>Ipoint = e.Intersect(pb);

            if(Ipoint.first==false){
                cout<<"edge and pb does not intersect, so both endpoint must be in same side\n";
                //does not intersect...
                //if both in same side as c site then keep the edge
                int cside = pb.getSide(c.site);
                int edgestartside = pb.getSide(e.st); //e.ed will also be in the same side
                int edgeendside = pb.getSide(e.ed);

                if( !isSameSide_not_Intersect(cside,edgestartside,edgeendside) ){
                    c.site.print();
                    cout<<" is not in the same side as ";
                    e.st.print();
                    cout<<" with respect to pb\n";
                    //delete this e
                    toDelete.push_back(i);
                    cout<<"will delete this edge\n";
                }
                //if both in same side as site then delete the edge
            }
            else{
                cout<<"edge and pb intersects at ";
                Ipoint.second.print();
                cout<<"\n";
                //intersects.. so, start and end point of edge e will change
                int cside = pb.getSide(c.site);
                int edgestartside = pb.getSide(e.st);
                int edgeendside = pb.getSide(e.ed);
                //this two will be in different sides with respect to cside

                if(edgestartside==0){
                    //either do nothing or delete
                    if(cside!=edgeendside)toDelete.push_back(i);
                }
                else if(edgeendside==0){
                    //similar case
                    if(cside!=edgestartside)toDelete.push_back(i);
                }
                else{
                    //intersects somewhere middle.. so need to clip
                    if(cside==edgestartside)e.clip(Ipoint.second,1);
                    else e.clip(Ipoint.second,0);

                    cout<<"edge is clipped to be ";
                    e.print("cell_edge_e");
                    c.regionEdges[i] = e;
                }

                X.push_back(Ipoint.second);
            }
        }

        for(int i = toDelete.size()-1; i>=0; --i){
            cout<<"deleting...............\n";
            int curdelIdx = toDelete[i];
            c.regionEdges.erase(c.regionEdges.begin()+curdelIdx);
        }

        /*cout<<"intersection vector size "<<(int)X.size()<<"\n";
        set<pair<double,double> >Xset;
        cout<<"points in vector\n";
        for(Point p: X){
            p.print();
            cout<<"\n";
            Xset.insert(make_pair(p.x,p.y));
        }
        cout<<"intersection set size "<<(int)X.size()<<"\n";*/

        for(int i = 0; i<X.size(); ++i){
            for(int j = (i+1); j<X.size(); ++j){
                Point p1 = X[i];
                Point p2 = X[j];
                if(fabs(p1.dist(p2))>eps){
                    Edge divider(p1,p2);
                    newcell.addEdge(divider);
                    c.addEdge(divider);
                }
            }
        }

        C[i] = c;
        cout<<"cell status\n";
        C[i].print_console(-1);
        cout<<"\n";

        cout<<"newcell status\n";
        newcell.print_console(-1);
        cout<<"\n";
    }
    cout<<op<<"\n";
    C.push_back(newcell);
    return;
}

void add_initial_site(double width, double height){
    Cell c1(S[0]);
    c1.addEdge(Edge(Point(0,0),Point(0,width)));
    c1.addEdge(Edge(Point(0,width),Point(width,height)));
    c1.addEdge(Edge(Point(width,height),Point(0,height)));
    c1.addEdge(Edge(Point(0,height),Point(0,0)));
    C.push_back(c1);
}

int main(){
    int n,width,height;
    scanf("%d %d %d",&n,&width,&height);
    takeSitePoints(n); //line 1-2

    add_boundary_Cells(width,height);

    //add_initial_site(width,height);

    for(int i = 0; i<S.size(); ++i){
        process_new_site(S[i]);
    }

    //C now should contain all voronoi cells
    for(int i = 0; i<C.size(); ++i){
        C[i].print(i);
    }

    /*ofstream fout("test.out");

    fout << "[ ";
    for(int i = 0; i<E.size(); ++i){
        Edge e = E[i];
        Point p1 = e.st;
        Point p2 = e.ed;
        fout<< "(" << p1.x<<","<<p1.y<<"), ("<<p2.x<<","<<p2.y<<")";
        if (i == E.size() - 1) fout << "]";
        else fout << ", ";
    }
    fout << endl;
    fout.close();*/
    fout.close();
    return 0;
}
