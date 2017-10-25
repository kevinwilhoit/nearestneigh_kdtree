/************************
Kevin Wilhoit
CpSc 212
Homework Assignment #3

Takes a few seconds to run, but it does finish. Is not an infinite loop.
************************/

#include <iostream>
#include <fstream> 
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <algorithm>

using namespace std;

double** a, e=2.71828182845904523536, alpha=.05;

struct Node{
  int key;
  int size;
  double weight;
  Node *left;
  Node *right;
  Node *parent;
  Node (int k, int n, Node *Tp) { key = k; size = 1; left = right = NULL; weight = 1.0/(double)n; parent = Tp;}
};
Node *insert(Node *T, int k, double splitval, int j, Node *Tp, int n){
  if (T == NULL) return new Node(k, n, Tp);
  T->size++;
  if (splitval < a[T->key][j]) T->left = insert(T->left, k, splitval++, j, T, n);
  else T->right = insert(T->right, k, splitval++, j, T, n);
  return T;
}
void print_inorder(Node *T, int d){
  int i=0;
  if (T == NULL) return;
  print_inorder(T->left, d);
  while (i<d) {cout << a[T->key][i] << " "; i++;}
  cout << endl;
  print_inorder(T->right, d);
}
Node *find(Node *T, int k){
  if (T == NULL) return NULL;
  if (k == T->key) return T;
  if (k < T->key) return find(T->left, k);
  else return find(T->right, k);
}
void meanvariance(int j, int i, int n, int d){
  double sum=0.0, mean=0.0;
  for (i=0; i < n; i++) {sum = sum + a[i][j];}
  mean = sum/n;
  sum=0;
  for (i=0; i < n; i++) {a[i][j] = a[i][j] - mean;}
  for (i=0; i < n; i++) {sum = sum + (a[i][j] * a[i][j]);}
  for (i=0; i < n; i++) {a[i][j] = a[i][j] / (sqrt(sum/(n-1)));}
}
double distfunc(int i, int j, int d){
  int k;
  double dist = 0;  
  for (k=0; k < d; k++) dist = dist + ((a[i][k] - a[j][k])*(a[i][k] - a[j][k]));
  return(sqrt(dist));
}
double nearestneighbors(int n, int d, Node *bst, int k){
  Node *T;
  int i=0, j=0, r=0, c[k], indexes[n];
  double avgerr=0, q=0, z=0, g[n], b[n]; 
  for (i=0; i < n; i++) b[i]=0;  
  for (i=0; i < k; i++) c[i]=0;
  for (i=0; i < n; i++){
    for (j=0; j < n; j++){
	  if (j == i) j++;
	  if (j != n){
	    T = find(bst, j);
	    b[j] = pow(e, (-1)*((alpha)*(distfunc(i,j,d))));
	    if (T != NULL){
	      indexes[j] = T->key;
	    }
	  }
	}
    for (r=0; r < n; r++){
      if (b[r] > c[0]) c[0] = indexes[r];
	  sort(c, c + k);
    }
	for (r=0; r < k; r++){
	  q += a[c[r]][0]; 
	  z = z + (b[c[r]] * a[c[r]][0]);
	}
	g[i] = q/z;
  }
  for (i=0; i < n; i++){
    avgerr += ((a[i][0] - g[i])*(a[i][0] - g[i]));
  }
  avgerr = avgerr / n;
  return(avgerr);
}
int main (int argc, char**argv){
  char *inputfile = argv[1];
  int k=atoi(argv[2]), n=0, d=0, l=0, p=0, i=0, j=0;
  while(k < 1 || k > 10){
    cout << "Please enter a value between 1 and 10 for k" << endl;
    cin >> k;
  }
  ifstream fin;
  fin.open(inputfile);
  fin >> n;
  fin >> d;
  a = new double*[n];
  for(i=0; i < n; i++) a[i] = new double[d+1];
  double t=0, mean=0, avgerr=0;
  while (fin >> t){
    if(p == d+1) {p=0; l++;}
	a[l][p] = t;
	p++;
  }
  fin.close();
  for (j=0; j < d+1; j++) meanvariance(j, 0, n, d+1);
  Node *bst = NULL;
  for (i=0; i < n; i++){
    j=0;
    while (j < d+1) {bst = insert(bst, i, a[i][j], j, NULL, n); j++;} 
  }
  avgerr = nearestneighbors(i, d+1, bst, k);
  cout << "Alpha parameter is: " << alpha << endl;
  cout << "Average Squared Error is: " << avgerr << endl;
  for (i=0; i<n; i++){delete a[i];}
  delete[] a;
}