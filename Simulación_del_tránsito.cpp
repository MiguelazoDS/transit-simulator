#include <iostream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include<iomanip>
#include <windows.h>
#include<conio.h> 

//Global
using namespace std;
int **MatrizPeso;// Matriz de Pesos
int const n=4;
int const CapAvenida=3;//cuando existe una avenida la capacidad se multiplica por capAvenida(puede ser 2 o 3)
int iter=0;
int iterTotal=1;
string it="1";
int llegadas_total=0;
int autosAgregados=0;
int id =0;
int cantidad_calles=0;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// Acontinuacion se define una classe Lista1 del tipo nodo1 para definir una Cola1  
// del tipo int. Esta cola va ser usada para almacenar las dirreciones
// ( id de los nodos) para el auto. Las dirreciones van a ser subministradas   
// por el algoritmo de dijstrak.
   
typedef int tipolista;

// Tamaño de la ciudad
int tamano_ciudad;//Nodos en uso (varia segun sea la ciudad)
    
    
class Nodo1{
    protected: 
        tipolista dato;
        Nodo1 *next;
    public:
        Nodo1() {next=NULL;};
        Nodo1(tipolista a) {dato=a; next=NULL;};
        void set_dato(tipolista a) {dato=a; };
        void set_next(Nodo1 *n) {next=n; };
        tipolista get_dato() {return dato; };
        Nodo1 *get_next() {return next; };
        bool es_vacio() {return next==NULL;};
        
};

class Lista1{
    protected: Nodo1 *czo;
    public:
            Lista1() {czo=new Nodo1();};
            Lista1(Nodo1 *n) {czo=n;};
            void del(void);
            void add(tipolista d);
            bool esvacia(void);
            tipolista cabeza(void);
            Lista1 *resto(void);
            string toPrint(tipolista p);   
            Nodo1 *get_czo(void){return czo;}
};

void Lista1::del(void)
{    Nodo1 *aux;
     aux=czo;
     czo=czo->get_next();
     delete aux;
}
void Lista1::add(tipolista d)
{  
     Nodo1 *nuevo=new Nodo1(d);
     nuevo->set_next(czo);
     czo=nuevo;
}
bool Lista1::esvacia(void)
{   
    return czo->es_vacio();
}

tipolista Lista1::cabeza(void)
{ 
  if(esvacia()){
                cout<<" Error, Cabeza de lista vacia";
                return -1; 
  }
  return czo->get_dato();
}

Lista1 *Lista1::resto(void)
{ 
      Lista1 *l=new Lista1(czo->get_next());
      return (l);
}

string Lista1::toPrint(tipolista p)
{ 
     if (this->esvacia()) {
        return "";
     } else {
       std::ostringstream stm;
       stm << this->cabeza()<<"-"<< this->resto()->toPrint(p) << endl;
       return stm.str();
     }
}

//------------------------------------------------------------------------

class Cola1{
      private:
              Lista1 *l;
              Nodo1 *czoq;
              void update_start(Lista1 *l,Nodo1 *p);
              void do_desencolar(Nodo1 *p,Nodo1 *s);
      public:
      Cola1(void){l=new Lista1();update_start(l,NULL);};
      ~Cola1(void) {delete(l);};
      int tope();
      int colavacia() { return l->esvacia();};
      void encolar(int a) ;
      void desencolar();
      Nodo1 *getCzoq(){ return czoq;};
      Lista1 *getLista(){return l;};
};
int Cola1::tope() {
           if (colavacia()){
              cout<<"Error: Cola vacia"<<endl;
              return -1;
           } 
           else
              return czoq->get_dato();
}

void Cola1::encolar(int a) {
     l->add(a);
     if (czoq==NULL) {
        update_start(l,czoq);
     }
}

void Cola1::desencolar() {
           if (colavacia())
              cout<<"Error: Cola vacia"<<endl;
           else {
              do_desencolar(l->get_czo(),NULL);
              update_start(l,NULL);
           }
}

void Cola1::do_desencolar(Nodo1 *p,Nodo1 *s){
     if (p->get_next()==NULL) {
         if (s!=NULL) {
            s->set_next(NULL);
         }
         delete(p);
     }else {
         this->do_desencolar(p->get_next(),p);
     }
}
void Cola1::update_start(Lista1 *l,Nodo1 *p){
     if (l->esvacia()) {
          czoq=p;
     }else {
          this->update_start(l->resto(),l->get_czo());
     }
}

// LA CLASE AUTO. Tiene una cola con el camino que tiene que seguir. La
// informacion de la cola se la proposiona el algoritmo de busqueda, esto se da 
// cuando se crea el objeto auto correspondiente(al principio de la execucion).
// Tiene un int, "prioridad", que va valer 1 para ambulancias y policias y 0
// para los demas.
// Tiene un int, "id", clave unica de identificacion.

class Auto { 
            private:
                   Cola1 *camino;
                   int prioridad;
                   int id;
                   bool avance;
            public:
				Auto(){camino = new Cola1(); avance = true;};
				int getPrioridad(){return prioridad;};
				int getId(){return id;};
				bool getAvance(){return avance;}
				void setPrioridad(int p){prioridad =p;};
				void setId(int x){id=x;};
				void setAvance(bool av){avance= av;};
				void setCamino(int x){camino->encolar(x);};
				void desencolarCamino(){camino->desencolar();};//Desencola el camino
				int topeCamino(){return camino->tope();};//Retorna el tope de la cola del camino
				void mostrarCamino(ofstream *,bool);//Muestra el camino completo del auto
				bool caminoVacio(){return camino->colavacia();};	
			
       };
void Auto::mostrarCamino(ofstream *fout=NULL, bool mostrar=false)
{
	 int cam[cantidad_calles];
     int x=0;
     Nodo1 *na;//nodo auxiliar para recorrer la cola
     na= camino->getLista()->get_czo();
	 while(na->get_next()!=NULL)
	 {
			//cout<<na->get_dato();
			cam[x]=na->get_dato(); x++;
			na=na->get_next();
			//if(na->get_next()!=NULL){cout<<"-->";}
	 }
	 for(int i=x-1; i>=0; i--)
	 {
			if(mostrar==true){cout<<cam[i];}
			if(fout!=NULL){*fout<<cam[i];}
			if((i-1)>=0)
			{if(mostrar==true){cout<<"-->";}
			 if(fout!=NULL){*fout<<"-->";}}
	 }
}

// La clase "calle" es el nexo entre el grafo, los autos y las calles.
// La matriz adyacente utilizada para crear el grafo va ser del tipo "calle".
// Tiene un arreglo del tipo Autos, donde se van a almacenar los autos en ese calle.
// es un arreglo para poder implementar en heapsort. 
// Tiene un double donde se define el "capacidad" de la calle, n° de carriles.
// Tiene un double "uso" para mostrar la cantidad de autos presentes en la "calle".
// Tiene un double "ocupacion" donde se relaciona la "capacidad" y "uso".
// Tiene un int "existe" si es 1 existe la calle si no es 0;

class calle{
	 private:  
       int existe;
       double capacidad;//Capacidad Total por calle--> Se amolda segun tipo de calle.[capacidad por carril (3*n)]
       Auto autos_en_calle[3*n*CapAvenida];// [3*n*CapAvenida]: es la capacidad maxima de autos por calle(solo las avenidas usaran la capacidad maxima)
       double uso;
       double ocupacion;
       int pasoN;// Contador de autos q aingresan a la calle
	   string tipo; //Tipo de calle (AVENIDA-->Capacidad de 3 carriles || COMUN--> Capacidad de un carril)
	public:
		int getExiste(){return existe;};
		double getCapacidad(){return capacidad;};
		Auto *getAuto(int dir){return &autos_en_calle[dir];};//Retorna el auto original de la "cola"(arreglo) segun la direccion dir
		Auto getCopiaAuto(int dir){return autos_en_calle[dir];};//Retorna una copia de cualquiera de los autos q se encuentran en el arreglo
		Auto *getAutosEnCalle(){return &autos_en_calle[0];};//Retorna un puntero a la primera direccion del arreglo
		double getUso(){return uso;}
		double getOcupacion(){return ocupacion;};
		int getPasoN(){return pasoN;};
		string getTipoCalle(){return tipo;}
		void setExiste(int e){existe=e;};
		void setCapacidad(double c){capacidad=c;};
		void agregarAutoEnCalle(Auto a,int dir){autos_en_calle[dir]=a;};//Agrega los autos al arreglo "cola"
		void disminuirUso(int du){uso=uso-du;};
		void setUso(double u){uso=u;};
		void aumentarUso(int au){uso=uso+au;};
		void setOcupacion(double o){ocupacion=o;};
		void setPasoN(int p){pasoN=p;};
		void aumentarPasoN(int ap){pasoN=pasoN+ap;};
		void setTipoCalle(string t){tipo=t;};
		
       };

class AuxOrdenarSemaforos{
      private:
      		int j;
      		int i;
	   public:
			int getI(){return i;};
			int getJ(){return j;};
			void setI(int ii){i=ii;};
			void setJ(int jj){j=jj;};
			void aumentarI(){i=i+1;};
			void disminuirI(){i=i-1;};
			void aumentarJ(){j=j+1;};
			void disminuirJ(){j=j-1;};
      };

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//Clase nodo necesaria para implementar Dijkstra.
class nodo
{
           private:
                   int nomVert;
                   int vertAnt;
                   int distOrigen;       
                   nodo *next;
           public:
                  nodo(){next=NULL;};    
                  nodo(int nv, int va, int dor){nomVert=nv; vertAnt=va; distOrigen=dor; next=NULL;};       
                  void set_nomV(int n) {nomVert=n; };
                  void set_vertA(int n) {vertAnt=n; };
                  void set_distOrigen(int n) {distOrigen=n; };
                  void set_next(nodo *n) {next=n; };
                  int get_nomV() {return nomVert; };
                  int get_vertA() {return vertAnt; };
                  int get_distOrigen() {return distOrigen; };
                  nodo *get_next() {return next; };
                  bool es_vacio() {return next==NULL;};
};

//Se creará una lista para guardar los nodos visitados.
class Lista{
    protected: nodo *czo;
    public:
            Lista() {czo=new nodo();};
            Lista(nodo *n) {czo=n;};
            void del(void);
            void add(nodo a);
            bool esvacia(void);
            int cabeza(void);
            Lista *resto(void);  
            nodo *get_czo(void){return czo;}
};

void Lista::add(nodo d)
{  
     nodo *nuevo=new nodo(d);
     nuevo->set_next(czo);
     czo=nuevo;
}

bool Lista::esvacia(void)
{   
    return czo->es_vacio();
}

int Lista::cabeza(void)
{ 
  if(esvacia()){
                cout<<" Error, Cabeza de lista vacia";
                return -1; 
  }
  return czo->get_nomV();
}

Lista *Lista::resto(void)
{ 
      Lista *l=new Lista(czo->get_next());
      return (l);
}

//En una cola se guardarán los nodos aún no visitados con orden de prioridad, este orden
//se basa en la distancia al origen.
class Cola{
      private:
              Lista *l;
              nodo *czoq;
              void update_start(Lista *l,nodo *p);
              void do_desencolar(nodo *p,nodo *s, nodo &a);
      public:
      Cola(){l=new Lista();update_start(l,NULL);};
      ~Cola(void) {delete(l);};
      int tope();
      int colavacia() { return l->esvacia();};
      void encolar(nodo a) ;
      void desencolar(nodo &a);
      
};

int Cola::tope() {
           if (colavacia()){
              cout<<"Error: Cola vacia"<<endl;
              return -1;
           } 
           else
           {    
               cout<<"\nVertice: "<<czoq->get_nomV()<<endl
                   <<"\nDistancia al origen: "<<czoq->get_distOrigen();
              return czoq->get_nomV();
           }
}

void Cola::encolar(nodo a) {
     l->add(a);
     if (czoq==NULL) {
        update_start(l,czoq);
     }    
}

void Cola::desencolar(nodo &a) {
           if (colavacia())
              cout<<"Error: Cola vacia"<<endl;
           else {
              do_desencolar(l->get_czo(),NULL,a);
              update_start(l,NULL);
           }
}

void Cola::do_desencolar(nodo *p,nodo *s, nodo &a){
     if (p->get_next()==NULL) {
         if (s!=NULL) {
            s->set_next(NULL);
         }         
         a=*czoq;
         delete(p);
     }else {
         this->do_desencolar(p->get_next(),p, a);
     }
}

void Cola::update_start(Lista *l,nodo *p){
     if (l->esvacia()) {
          czoq=p;
     }else {
          this->update_start(l->resto(),l->get_czo());
     }
}



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------


//Actualiza la distancia mínima.
void actDistMin(nodo &act, int vOr, int cantV)
{
     if((MatrizPeso[(vOr-1)][(act.get_nomV()-1)]!=0)&&(MatrizPeso[(vOr-1)][(act.get_nomV()-1)]<act.get_distOrigen()))
     {
          act.set_distOrigen(MatrizPeso[(vOr-1)][(act.get_nomV()-1)]);
          act.set_vertA(vOr);
     }

}

//Desencola el nodo con la distancia más cercana al origen.
void desencolarMinimo(Cola *c,nodo &min,int &cont)
{     
     nodo temp1,temp2;
     int contaux=cont-1;   
     
     if(cont==1)
     {
          c->desencolar(min);
          cont--;
     }
     else
     {
          c->desencolar(min);
          cont--;
          for(int i=0;i<contaux;i++)
          {               
               c->desencolar(temp1);
               cont--;
               if(min.get_distOrigen()<=temp1.get_distOrigen())
               {
                    c->encolar(temp1);
                    cont++;               
               }
               else
               {
                    temp2=temp1;
                    temp1=min;
                    min=temp2; 
                    c->encolar(temp1);  
                    cont++;       
               }
          }
     }       
}

//Implementación del algoritmo de Dijkstra.
void Dijkstra(int cantV, int vertOr, int vertDes, Auto *autoGenerico)
{
     int cont=0;
     bool valor=true;        
     
     //Crea un arreglo de nodos para almacenar los nodos con distancia mínima ya calculada. 
     nodo array[cantV]; 
     //nodo arrayCalc[cantV];
     nodo *arrayCalc=new nodo[cantV];
     //Colocar nombres.
     for(int i=0;i<cantV;i++)
     {
          array[i].set_nomV(i+1);
          array[i].set_distOrigen(-1);
          array[i].set_vertA(-1);
     }   
      
     //Crea una cola para los nodos pendientes de evaluar.
     Cola *c=new Cola; 
                  
     array[vertOr-1].set_distOrigen(0);  
     //Inserta el nodo origen en la cola de prioridad. 
     c->encolar(array[vertOr-1]); 
     cont++;    
     //mientras haya nodos en la cola de prioridad.
     nodo min;       
     int j=0; 
     bool igual; 
     bool marcado;
     
     while(!c->colavacia())
     {                   
          
          desencolarMinimo(c, min, cont);
          marcado=true;
          for(int k=0;k<j;k++)
          {                         
               if(arrayCalc[k].get_nomV()==min.get_nomV())
               {
                    marcado=false;
               }
          }            
          if(marcado)                                 
          {    
               arrayCalc[j]=min;
               j++; 
          
               for(int i=0;i<cantV;i++)
               {
                    if(MatrizPeso[min.get_nomV()-1][i]!=0)
                    {                                                          
                         igual=true;                    
                         for(int k=0;k<j;k++)
                         {                         
                              if(arrayCalc[k].get_nomV()==(i+1))
                              { 
                                   igual=false;
                              }
                         }
                         if(igual)
                         { 
                              array[i].set_distOrigen(MatrizPeso[min.get_nomV()-1][i]+min.get_distOrigen());
							  array[i].set_vertA(min.get_nomV());                                             
                    
                              if(min.get_nomV()==vertOr)
                              {
                                   c->encolar(array[i]);
                                   cont++;
                              }  
                              else
                              {
                                   actDistMin(array[i], vertOr, cantV);
                                   c->encolar(array[i]);
                                   cont++;
                              }
                         }
                    } 
               }               
          } 
     }
         
     //muestra el camino
     int k=vertDes;
     int l=j;
     int i=0;
     int guardar[j];
     
     while(k!=-1)
     {
          if(arrayCalc[i].get_nomV()==k) 
          {
               guardar[l-1]=arrayCalc[i].get_nomV();
               k=arrayCalc[i].get_vertA();    
               i=0;
               l--;
               if(l<0)
                    k=-1;
          }
          else
               i++;
          if(i>j)
               k=-1;                         
     }
     
     for(int i=l;i<j;i++)
     {
          if(i!=l){   
          autoGenerico->setCamino(guardar[i]);}
          i++;
          i--;        
     }
     if(autoGenerico->caminoVacio()==true){cout<<" Origen sin salidas.\n";}
}

//IMPORTANTE: AQUI SE DIMENSIONAN TODAS LAS MATRICES(ADYACENCIA, PESOS, GRAFO)
int RedimMatriz(int **&MA, int **&MatrizPeso, calle **&grafo, int cantVertices)
{                 
     MA = new int *[cantVertices];
     MatrizPeso= new int *[cantVertices];
     grafo = new calle *[cantVertices];
     for (int i = 0; i < cantVertices; i++){
          MA[i] = new int [cantVertices];
          MatrizPeso[i] = new int [cantVertices];
		  grafo[i] = new calle [cantVertices];}                                                                          
     
     return cantVertices;
}
//
void LeerArchivo(int **&MA, calle **&grafo)
{         
     string nombre; 
     int cantVertices=0;
     bool valor=true;
                       
     while(valor)
     {
          ifstream archivo_ent;                   
          cout<<"Ingrese el nombre del archivo para cargar una ciudad: "<<endl;
          getline(cin, nombre);
          nombre=nombre+".txt";                            
          archivo_ent.open(nombre.c_str());
          if(archivo_ent.fail())        
               cout<<"El archivo no se abrio correctamente, intentelo nuevamente "<<endl;                                          
          else
          { 
               cout<<"El archivo se abrio correctamente "<<endl; 
               while(archivo_ent.good())
               {                                                            
                    if(archivo_ent.get()=='\n'||archivo_ent.eof())
                    cantVertices++;
               } 
			   //IMPORTANTE: Aca se dimensiona la variable Nodos (q es la cantidad de vertices del grafo)
				tamano_ciudad=cantVertices;                               
               cantVertices=RedimMatriz(MA, MatrizPeso, grafo, cantVertices);
               archivo_ent.clear();
               archivo_ent.seekg(0,archivo_ent.beg);
               int k1=0;
               int k2=0;
               while(archivo_ent.good())
               {                                                                                
                    archivo_ent>>MA[k1][k2];
                    k2++;
                    if(archivo_ent.get()=='\n')
                    {
                         k1++;
                         k2=0;                    
                    }                                                                 
               }                                               
               valor=false;
          }
		  archivo_ent.close();//Finaliza lectura del archivo                      
     }        
}
// Esta es la funcion que modela nuestra ciudad a partir de MR, matriz de referencia
// dada por el profe o el archivo binario
void HacerGrafo(calle **&grafo, int **&MA){
	// los for recorren la MR y donde existe crea la calle en el grafo	
        for(int i=0; i<tamano_ciudad; i++)
		{
			for(int j=0; j<tamano_ciudad; j++)
	    	{
				if (MA[i][j]==1 && grafo[i][j].getExiste()!=1){
                                    cantidad_calles++;                                  
                                    grafo[i][j].setExiste(1);
                                    grafo[i][j].setUso(0);
                                    grafo[i][j].setOcupacion(0);
                                    if(MA[j][i]==1 && grafo[j][i].getExiste()!=1)
                                    {
                                    	grafo[i][j].setTipoCalle("AVENIDA");//se introduce Tipo de calle
										grafo[i][j].setCapacidad(CapAvenida*(3*n));//se introduce capacidad segun el tipo de calle
										//Se crea la calle de vuelta
										cantidad_calles++;                                  
                                    	grafo[j][i].setExiste(1);
                                    	grafo[j][i].setUso(0);
                                    	grafo[j][i].setOcupacion(0);
                                    	grafo[j][i].setTipoCalle("AVENIDA");
										grafo[j][i].setCapacidad(CapAvenida*(3*n));
									}
									if(MA[j][i]==0)
									{
										grafo[i][j].setTipoCalle("COMUN");
										grafo[i][j].setCapacidad(3*n);
									}
                                                                                           
                                    }
                                    if(MA[i][j]==0 && grafo[i][j].getExiste()!=1){
                                    grafo[i][j].setExiste(0);}
			}}}
// Imprime el grafo, como matriz de adyacencia. Parametro "Existe" del grafo.
void ImprimirGrafo(calle **&grafo, int o, bool mostrar=true){
    ofstream fout("MATRIZ_ADY.txt");
	if(tamano_ciudad >=15)
	{mostrar=false;cout<<"\nDebido al tamaño de la ciudad no se puede mostrar su matriz representativa\n";
	 cout<<"La misma se ha guardado en MATRIZ_ADY.txt para verificar si la carga fue exitosa.\n\n";}
	if(mostrar==true){cout<<endl;
	cout<<"El plano de la ciudad esta representado por la siguiente matriz de referencia\n"<<endl;}
	//Comienza escritura del archivo
	fout<<"El plano de la ciudad esta representado por la siguiente matriz de referencia\n"<<endl;
	for(int x=0; x<o; x++) 
	{
		if(mostrar==true){
			if(x==0){cout<<setw(5)<<x+1;}
			else {cout<<setw(4)<<x+1;}}
		if(x==0){fout<<setw(6)<<x+1;}
		if(x>0){fout<<setw(4)<<x+1;}
	}if(mostrar==true){cout<<endl;}
	fout<<endl;
		for(int i=0; i<o; i++)
		{
	    	if(mostrar==true){cout<<endl<<i+1;}
	    	fout<<endl<<setw(2)<<i+1;
			for(int j=0; j<o; j++)
	    	{
				if (grafo[i][j].getExiste()==0){
					fout<<setw(4)<< 0;				
                    if(mostrar==true){cout<<setw(4)<< 0;}}
                else{
					fout<<setw(4)<<1;
                    if(mostrar==true){cout<<setw(4)<<1;}}
			}
		}if(mostrar==true){cout<<"\n\n";}
		fout.close();
}
// funcion que define el origen y el destino de un auto
// origen y destino son una par ordenado de int (i,j)	
void OrigenDestino(int origen[2], int destino[2], calle **&grafo, Auto *autoGenerico){                                
                                  // origen[][] par ordenado. verifico si
                                  // existen en el grafo esa calle y no esta llena con un while
                                  int control = 0;
                                  while(control ==0){              
                                  	if(autoGenerico->getId()==1)
                                  	{
										cout<<"\nSu auto tiene id: 1\nA continuacion defina el origen en que desea ubicarlo: ";
										cin>>control;
										while(control>tamano_ciudad || control<1)
										{cout<<"\nNo existe el origen definido. Intentelo nuevamente con otro origen: ";
										 cin>>control;}
										origen[1]=control; origen[0] = rand()%tamano_ciudad+1;
										while(grafo[origen[0]-1][origen[1]-1].getExiste()!=1){origen[0] = rand()%tamano_ciudad+1;}
										while(grafo[origen[0]-1][origen[1]-1].getOcupacion() >= 100){
											cout<<"\nEl origen definido no existe o puede que se encuentre con capacidad colmada\n";
											cout<<"\nIntentelo nuevamente definiendo otro origen: ";
											cin>>control;
											while(control>tamano_ciudad || control<1)
											{cout<<"\nNo existe el origen definido. Intentelo nuevamente con otro origen: ";
										 	 cin>>control;}
											origen[1]=control; origen[0] = rand()%tamano_ciudad+1;
											while(grafo[origen[0]-1][origen[1]-1].getExiste()!=1){origen[0] = rand()%tamano_ciudad+1;}
										}
								  	}
								  	else
									{
										origen[0] = rand()%tamano_ciudad+1;
                                  		origen[1] = rand()%tamano_ciudad+1;
                                  		if(grafo[origen[0]-1][origen[1]-1].getExiste()==1 && grafo[origen[0]-1][origen[1]-1].getOcupacion() < 100){control++;}
								  	}
                                  }
                                  // dsetino[][] par ordenado. verifico si
                                  // existen en el grafo esa calle con un while
                                  control =0;
                                  while(control ==0){  
										if(autoGenerico->getId()==1)
                                  		{
											cout<<"\nDefina el destino a llegar: ";
											cin>>control; 
											while(control>tamano_ciudad || control<1)
											{
												cout<<"\nEl destino definido no existe. Intentelo nuevamente con otro destino: ";
												cin>>control;
											}
											destino[1]=control; destino[0] = rand()%tamano_ciudad+1;
											while(grafo[destino[0]-1][destino[1]-1].getExiste()!=1){
                                            destino[0] = rand()%tamano_ciudad+1;
                                            }
											while(destino[1] == origen[1]){
												cout<<"\nEl destino no puede ser igual al origen definido\n";
												cout<<"\nIntentelo nuevamente con otro destino: "; cin>>control;
												while(control>tamano_ciudad || control<1)
												{
													cout<<"\nEl destino definido no existe. Intentelo nuevamente con otro destino: ";
													cin>>control;
												}
												destino[1]=control; destino[0] = rand()%tamano_ciudad+1;
												while(grafo[destino[0]-1][destino[1]-1].getExiste()!=1){destino[0] = rand()%tamano_ciudad+1;}}
										}
										else
										{
											destino[0] = rand()%tamano_ciudad+1;
                                  			destino[1] = rand()%tamano_ciudad+1;
                                  			if(grafo[destino[0]-1][destino[1]-1].getExiste()==1 && destino[1]!= origen[1]){control++;}
										}
								  	}    
}    
// MATRIz DE PESOS
void CrearMatrizPeso(calle **&grafo, bool mostrar=true) {
     
	ofstream fout("MP.txt");
	string cadena="MPgraph_"+it+".txt";
	ofstream tout(cadena.c_str());
	if(tamano_ciudad >=15)
	{  //cout<<"\nLa matriz de pesos se guardo en "<<cadena<<".txt\n"<<endl;
	   mostrar=false;}
	if(mostrar==true){cout<<"           La matriz de peso es\n"<<endl;}
	//Comienza la escritura del archivo
	fout<<"La matriz de peso es\n"<<endl;
	for(int x=0; x<tamano_ciudad; x++) 
	{
		if(mostrar==true){
			if(x==0){cout<<setw(5)<<x+1;}
			else {cout<<setw(4)<<x+1;}}
		//Escritura sobre el archivo
		if(x==0){fout<<setw(6)<<x+1;}
		if(x>0){fout<<setw(4)<<x+1;}
	}if(mostrar==true){cout<<endl;}
     for(int i=0; i<tamano_ciudad; i++)
		{    
             if(mostrar==true){cout<<endl<<i+1;}
             fout<<endl<<setw(2)<<i+1;
             if(i>0)
             {tout<<endl;}
			for(int j=0; j<tamano_ciudad; j++)
	    	{	
				if (grafo[i][j].getExiste()==1){
                                    if(grafo[i][j].getOcupacion()<=25){MatrizPeso[i][j] = 25;}
                                    else {MatrizPeso[i][j] = (int)grafo[i][j].getOcupacion();}
                                    fout<<setw(4)<<MatrizPeso[i][j];
                                    tout<<MatrizPeso[i][j];
                                    if(j<tamano_ciudad-1)
                                    	 tout<<";";
									if(mostrar==true){cout<<setw(4)<< MatrizPeso[i][j];}}
                                    else {MatrizPeso[i][j] = 0;
                                    	 fout<<setw(4)<<MatrizPeso[i][j];
                                    	 tout<<MatrizPeso[i][j];
                                    	 if(j<tamano_ciudad-1)
                                    	 tout<<";";
                                         if(mostrar==true){cout<<setw(4)<<MatrizPeso[i][j];}}
            }			
   }if(mostrar==true){cout<<endl;}
   fout.close();//Finaliza escritura de archivo   
   tout.close();
     } 
//Funcion para agregar autos al grafo, la variable cantidad_de_autos define
// el numero de auto a agregar 
void AgregarAutos(calle **&grafo, int cantidad_de_autos){
                        
						int aux=0;
                        int origen[2]={0,0};
                        int destino[2]={0,0};
                        ofstream fout("Registro_Autos.txt",ios::app);
                        fout<<"REGISTRO DE AUTOS AGREGADOS\n\n";
                        
                        while(aux < cantidad_de_autos){
                                  //creo el auto
                                  Auto *autoGenerico = new Auto();                                
                                  // prioridad numero aleatorio 999 o 0
                                  if(1 == rand()%2){autoGenerico->setPrioridad(999);}
                                  else autoGenerico->setPrioridad(0);
                                  //establesco su id. la clave unica de identificacion
                                  id++;
                                  autoGenerico->setId(id);
                                  //                      IMPORTANTE
                                  //funcion que define el orgine y el destino
                                  OrigenDestino(origen, destino, grafo, autoGenerico);
                                  // Meto el auto en la arista del grafo. Y le ajusto
                                  //la prioridad con el "uso" de la calle Esto es porque
                                  //de esa forma queda cierto orden en la calle de acuerdo a como van llegando los autos.
                                  //una vez que el auto salga de esta calle tiene
                                  // que reiniciarse la prioridad a 0 o 999
                                  int aux2= (int)grafo[origen[0]-1][origen[1]-1].getUso();
                                  autoGenerico->setPrioridad(autoGenerico->getPrioridad()- aux2 + (int)grafo[origen[0]-1][origen[1]-1].getCapacidad() );
                                  
                                  //IMPORTANT LLAMAMOS DIJKSTRA ANTES DE GUARDAR EL AUTO EN EL GRAFO
								  //LLamo al algoritmo Dijkstra
                                  Dijkstra(tamano_ciudad, origen[1],destino[1], autoGenerico);// Para guardar camino
                                  
                                  //Guardo auto en cola de calle con todos los datos
                                  grafo[origen[0]-1][origen[1]-1].agregarAutoEnCalle(*autoGenerico, aux2);
                                  
                                  //Actualizo la informacion de la calle
                                  grafo[origen[0]-1][origen[1]-1].aumentarUso(1);
                                  grafo[origen[0]-1][origen[1]-1].setOcupacion(((grafo[origen[0]-1][origen[1]-1].getUso())/(grafo[origen[0]-1][origen[1]-1].getCapacidad())*100));
								  
								  //Imprimo la informacion del Auto Almacenado
								  if(cantidad_de_autos<11)
								  {
								  	cout << endl << "                    AUTO NUEVO" << endl;
                                  	cout << "Tiene id: " << autoGenerico->getId()<< endl; 
                                  	cout << "Se agrego un auto con origen en: [" << origen[0] << "]["<< origen[1] << "]" << endl;
                                  	cout << "Y destino: [" << destino[0] << "][" << destino[1] << "]" << endl;
                                  	cout << "Tiene prioridad: " << autoGenerico->getPrioridad()<< endl;
								  	cout<<"Su esquina origen es: "<<origen[1]<<endl; cout<<"Su esquina destino es: "<<destino[1];
								  	cout<<"\nSu camino de origen a destino es: "<<origen[1]<<"-->";autoGenerico->mostrarCamino(NULL,true);
								  }
                                  
                                  //Escritura en archivo
								  fout << endl << "                    AUTO NUEVO" << endl;
                                  fout << "Tiene id: " << autoGenerico->getId()<< endl; 
                                  fout << "Se agrego un auto con origen en: [" << origen[0] << "]["<< origen[1] << "]" << endl;
                                  fout << "Y destino: [" << destino[0] << "][" << destino[1] << "]" << endl;
                                  fout << "Tiene prioridad: " << autoGenerico->getPrioridad()<< endl;
                                  fout<<"Su esquina origen es: "<<origen[1]<<endl; fout<<"Su esquina destino es: "<<destino[1];
                                  fout<<"\nSu camino de origen a destino es: "<<origen[1]<<"-->";autoGenerico->mostrarCamino(&fout);
								  
                                  //Muestro informacion de calle
                                  if(cantidad_de_autos<11){
									
                                  	cout << endl;
                                  	cout << "\n                INFORMACION DE LA CALLE ["<<origen[0]<<"]["<<origen[1]<<"]"<< endl;
                                  	cout << "La calle tiene: " << grafo[origen[0]-1][origen[1]-1].getUso() << " autos. Y su ocupacion es de: ";
								  	cout<< grafo[origen[0]-1][origen[1]-1].getOcupacion()<< "%" << endl; 
								  	cout<<"Tipo de calle: "<< grafo[origen[0]-1][origen[1]-1].getTipoCalle()<<endl;
                                  	cout << endl;}
                                  
								  fout << endl;
                                  fout << "\n                INFORMACION DE LA CALLE ["<<origen[0]<<"]["<<origen[1]<<"]"<< endl;
                                  fout << "La calle tiene: " << grafo[origen[0]-1][origen[1]-1].getUso() << " autos. Y su ocupacion es de: ";
								  fout<< grafo[origen[0]-1][origen[1]-1].getOcupacion()<< "%" << endl;
								  fout<<"Tipo de calle: "<< grafo[origen[0]-1][origen[1]-1].getTipoCalle()<<endl;
                                  fout << endl;
                                  
                                  aux++;
                                  CrearMatrizPeso(grafo, false);
                                  }fout.close();
        if(cantidad_de_autos>10){cout<<"\nLos vehiculos agregados se encuentran en Registro_Autos.txt\n\n";}
     }

// Devuelve la cantidad de autos en el grafo/ciudad	
int CantidadAutos(calle **&grafo){
    int control=0;
    //recorro el grafo
    for(int i=0; i<tamano_ciudad; i++)
		{
			for(int j=0; j<tamano_ciudad; j++)
	    	{
				if (grafo[i][j].getExiste()==1){//voy sumando la cantidad de autos en las calles
                                           control = control + (int)grafo[i][j].getUso();
                                           }
                                           }}
    return control;
    }

//Funcion recursiva auxiliar de BuscarAuto, busca auto por id    
int BuscarAutoAux(Auto arregloAutos[], int idd, int i, int j, int control, int aux, int uso){
     
     // la calle esta vacia. no hace nada                            
     if (aux == uso){return 0;}
     // Encontro el Auto
     else if (idd==arregloAutos[aux].getId()) {
     cout << endl;
     cout << "Se encontro el Auto en la esquina que va de "<< i+1 << " a " << j+1 << endl;
     cout << "La prioridad del Auto es: " << arregloAutos[aux].getPrioridad() << endl;
     cout << "Su id es: " << idd << endl;
     if(arregloAutos[aux].getAvance()==true){cout << "Permiso de avance: TRUE"<<endl;}
     else{cout << "Permiso de avance: FALSE"<<endl;}
     cout<<"Su camino de origen a destino es: "<<j+1<<"-->";
     arregloAutos[aux].mostrarCamino(NULL,true);
     cout << "\nLa proxima direccion es: " << arregloAutos[aux].topeCamino()<< endl; 
     control = 1;
     return control;
     }
     //Llamada recursica. Lo unico que cambia es aux, le pasa ++aux para ir recorriendo el arreglo     
     else  return BuscarAutoAux(arregloAutos, idd, i, j,control, ++aux, uso);
     }
     
//busca un auto por id(clave unica de identificacion. En todo el grafo.
int BuscarAuto(calle **&grafo, int idd){
                       //RECORRE el grafo en busca del auto
                       int Aux=0;
                       for(int i=0; i<tamano_ciudad; i++)
		               {
			           for(int j=0; j<tamano_ciudad; j++)
	    	           {
				       if (grafo[i][j].getExiste()==1){
                                                 //Llamada a una funcion recursiva auxiliar. que se le manda un arreglo de los autos  
                                                 Aux = BuscarAutoAux(grafo[i][j].getAutosEnCalle(), idd, i, j,0, Aux, (int)grafo[i][j].getUso());
                                                 if(Aux==1){return 1;}
                                                 }
                                                 }
                                                 }   
           cout << endl << "El Auto con ese id no existe" << endl;
           return 0;              
           }
     

//Funcion que Ordena los Semaforos/calles.
//Los semaforos/calles quedan ordenados y guardados en SemaforosOrdenados[] 
// que es un arreglo de la clase AuxOrdenarSemaforos. Tiene dos campos i j
// los i j hacen referencia a el grafo.
void OrdenarSemaforos(calle **&grafo, AuxOrdenarSemaforos *SemaforosOrdenados, bool mostrar=true){
     
     ofstream fout("Semaforos_Ordenados.txt");
	 fout<<"REGISTRO DE LOS SEMAFOROS ORDENADOS SEGUN LA CARGA DE LAS CALLES\n"<<endl;
	 //Variables Auxiliares para el metodo de ordenamiento de seleccion directa 
     int aux=-1, pos=0, SO=0;// SO: Semaforos ocupados
     AuxOrdenarSemaforos men;
     
     //RECORRE el grafo.Guarda los i j de las calles(encualquier orden) para despues ordenarlos.
     for(int i=0; i<tamano_ciudad; i++)
		    {
			   for(int j=0; j<tamano_ciudad; j++)
	    	           {
				       if (grafo[i][j].getExiste()==1){
                                                  aux++;                
                                                  //Guardo los i j de las calles(encualquier orden) para despues ordenarlos.
                                                  SemaforosOrdenados[aux].setJ(j);
                                                  SemaforosOrdenados[aux].setI(i);                                             
     }
     }
     }      
     //Ordena el arreglo SemaforosOrdenados
     //Metodo Seleccion Directa
     for(int i=0;i<aux;i++){
     men.setI(SemaforosOrdenados[i].getI());
     men.setJ(SemaforosOrdenados[i].getJ());
     pos=i;
     
     for(int j=i+1;j<=aux;j++){
     if((grafo[SemaforosOrdenados[j].getI()][SemaforosOrdenados[j].getJ()].getOcupacion()) < (grafo[men.getI()][men.getJ()].getOcupacion())){
       men.setI(SemaforosOrdenados[j].getI());
       men.setJ(SemaforosOrdenados[j].getJ());
       pos=j;      
       }
       }
       SemaforosOrdenados[pos].setI(SemaforosOrdenados[i].getI());
       SemaforosOrdenados[pos].setJ(SemaforosOrdenados[i].getJ());
       SemaforosOrdenados[i].setI(men.getI());
       SemaforosOrdenados[i].setJ(men.getJ());
       }
      
      //Imprimo el ranking de semaforos
      if(mostrar==true){
	  for(int i=0;i<aux+1;i++){
                         cout << "Ranking " << aux-i+1 << endl;
                         cout << "El semaforo que va de " << (SemaforosOrdenados[i].getI())+1 << " a " << (SemaforosOrdenados[i].getJ())+1 << endl;
                         cout << "Y su ocupacion es de: " << (grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getOcupacion()) << endl;
                         }    
     //Los semaforos/calles quedan ordenados y guardados en SemaforosOrdenados[] a traves de sus i j  
     }
     
     for(int i=0;i<aux+1;i++){
                         fout << "Ranking " << aux-i+1 << endl;
                         fout << "El semaforo que va de " << (SemaforosOrdenados[i].getI())+1 << " a " << (SemaforosOrdenados[i].getJ())+1 << endl;
                         fout << "Y su ocupacion es de: " << (grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getOcupacion()) << endl<<endl;
                         //Contador de semaforos ocupados
                         if(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getOcupacion()>0){SO++;}
                         }
    fout<<"\n\nCANTIDAD DE SEMAFOROS CON COLAS DE AUTOS: "<<SO<<endl;
    fout<<"CANTIDAD DE SEMAFOROS SIN COLAS DE AUTOS: "<<cantidad_calles - SO<<endl;
    fout<<"CANTIDAD DE SEMAFOROS EN LA CIUDAD: "<<cantidad_calles<<endl;
    
    fout.close();
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

// |||||||||||PARTE DEL HEAP SORT APLICADO EN UN ARREGLO

void Maxheapify(Auto *A,int i,int tamano)
{
    int l=i;
    // SE COMPAran la prioridades
    if(2*i <tamano && (A[i].getPrioridad() < A[2*i].getPrioridad() || A[i].getAvance()== false ) && A[2*i].getAvance()== true)
    {
        l=2*i;
    }
    if(2*i+1 <tamano && (A[l].getPrioridad() < A[2*i+1].getPrioridad() || A[l].getAvance()== false ) && A[2*i+1].getAvance()== true)
    {
        l=2*i+1;
    }
    if(l!=i)
    {
        Auto t=A[i];
        A[i]=A[l];
        A[l]=t;
        Maxheapify(A,l,tamano);
    }
}

//Se empieza a crear el Heap(arbol binario-> se ordena el arreglo en base a un arbol binario)
void Crearmaxheap(Auto *A,int tamano)
{    
    for(int i=tamano/2;i>=0;i--)
    {
        Maxheapify(A,i,tamano);
    }
}

//Funcion principal del heapsort
void heapsort(calle **&grafo, bool mostrar=true)
{     
      
    ofstream fout("AutosOrdenados_MenorMayor.txt");
    fout<<"REGISTRO DE AUTOS ORDENADOS DE MENOR A MAYOR\n"<<endl;
	      
    //LOS DOS FOR y el IF recorren el grafo calle por calle ordenando los autos.
    for(int ii=0; ii<tamano_ciudad; ii++)
		               {
			           for(int jj=0; jj<tamano_ciudad; jj++)
	    	           {
				       if (grafo[ii][jj].getExiste()==1){
                                                      
    //Se Crea el heap   
    Crearmaxheap(grafo[ii][jj].getAutosEnCalle(),(int)grafo[ii][jj].getUso());

    int tamano=(int)grafo[ii][jj].getUso();
    int MaxAux = (int)grafo[ii][jj].getUso();
    //HeapSort propiamente dicho
    for(int i=MaxAux-1;i>=1;i--)
    {       
        //Al primer valor del heap (el mas grande) lo pongo atras, no se vuelve a tocar
        Auto t=grafo[ii][jj].getCopiaAuto(0);
        grafo[ii][jj].agregarAutoEnCalle(grafo[ii][jj].getCopiaAuto(i),0);
        grafo[ii][jj].agregarAutoEnCalle(t, i);
        tamano--; 
        //Reorganizo el heap disminuyendo el tamaño en uno, para q el 
        //que "saque" recien no lo toque y va quedando org de menor a mayor.
        Maxheapify(grafo[ii][jj].getAutosEnCalle(),0,tamano);
    }
    
    // imprimo la iformacion de la calle y los autos en orden
    if(mostrar==true){
	cout<<endl;
    cout << "La calle de " << ii+1  << " a " << jj+1<<" (calle "<<grafo[ii][jj].getTipoCalle() << ") tiene " << grafo[ii][jj].getUso() << " autos" << endl;
    // imprime autos en orden
    for(int i=0; i<grafo[ii][jj].getUso(); i++){
           
		  	cout << "AUTO con id: " << grafo[ii][jj].getAuto(i)->getId() << endl; 
          	cout << "Tiene prioridad: " << grafo[ii][jj].getAuto(i)->getPrioridad() << endl;
          	cout << endl;
          }
		}
		
	fout<<endl;
    fout << "La calle de " << ii+1  << " a " << jj+1<<" (calle "<<grafo[ii][jj].getTipoCalle() << ") tiene " << grafo[ii][jj].getUso() << " autos" << endl;
    // imprime autos en orden
    for(int i=0; i<grafo[ii][jj].getUso(); i++){
          
          fout << "AUTO con id: " << grafo[ii][jj].getAuto(i)->getId() << endl; 
          fout << "Tiene prioridad: " << grafo[ii][jj].getAuto(i)->getPrioridad() << endl;
          fout << endl;
          }
}
}
}
fout.close();
}

//Esta funcion actualiza el avance de todos los autos con true y reinicia los pasosN a 0
void ActualizarAvance(calle **&grafo){
    int control=0;
    //recorro el grafo
    for(int i=0; i<tamano_ciudad; i++)
		{
			for(int j=0; j<tamano_ciudad; j++)
	    	{
				if (grafo[i][j].getExiste()==1)
				{
					grafo[i][j].setPasoN(0);
					for(int x=0; x<grafo[i][j].getUso(); x++)
					{
						grafo[i][j].getAuto(x)->setAvance(true);//variable avance en true
					}
                }
    		}
		}
}

//Funcion que lleva acabo las Itereaciones
void Iteracion(calle **&grafo, int cantidad_de_iteraciones,AuxOrdenarSemaforos *SemaforosOrdenados){
	
	int llegadas=0, calles_vacias=0;
	OrdenarSemaforos(grafo,SemaforosOrdenados,false);
	ActualizarAvance(grafo);//se posibilita el avance de todos los autos y se reinicia el pasoN a 0
	heapsort(grafo, false);
	iter++; iterTotal++;
	stringstream convertir;// convierte los int a string
	convertir.str("");
	convertir<<(iterTotal);
	it= convertir.str();//int metido en un string
	ofstream fout("Movimiento_de_Vehiculos.txt");
	fout<<"REGISTRO DE MOVIMIENTOS DE VEHICULOS\n\n";
	
	for(int i=(cantidad_calles-1); i>=0; i--)
	{
		
		//heapsort(grafo, false);
		int capDisp=0;//Capacidad disponible de cada calle destino
		int nn=n;// limete de autos a pasar segun sea AVENIDA o calle  COMUN
		bool paso=false;//Indica si el pasoN de una calle ha aumentado(TRUE:si aumento||FALSE:no aumento) necesario if Nro.8
		int usoO;// uso de calle origen
		int usoD;// uso de calle destino
		int camino;
		
		
		usoO= (int)(grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].getUso());
		
		while((grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getUso())>0 && usoO>0){
	    
		heapsort(grafo, false);
		if(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->getAvance() ==true){
				
			//Control para recacular camino de nuestro auto
			if(iter>2 && grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->getId()==1)
			{
				SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x03);
				cout<<"---------------------------------------------------"<<endl;
				fout<<"\nAuto id: 1\nSe verificara el camino nuevamente.";//Escritura en archivo
				cout<<"\n Auto id: 1\n Se verificara el camino nuevamente.";
				int destino;//Variable para guardar el destino a llegar
				while(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->caminoVacio()==false)
				{
					destino=(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->topeCamino());//guardo destino(cuando se vacie la cola queda almacenado el destino) no le sumo 1 ya q dijkstra se lo resta
					grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->desencolarCamino();//Descarto camino para recalcular uno nuevo
				}
				//Aqui se recalcula el camino de nuestro auto
				fout<<"\nSu auto con id:1 ya tiene nuevo camino.\n";
				cout<<"\n Su auto con id:1 ya tiene nuevo camino.\n";
				Dijkstra(tamano_ciudad, SemaforosOrdenados[i].getJ()+1,destino,grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1));
				fout<<"El camino de origen a destino ahora es: "<<SemaforosOrdenados[i].getJ()+1<<"->";
				cout<<" El camino de origen a destino ahora es: "<<SemaforosOrdenados[i].getJ()+1<<"->";
				grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->mostrarCamino(&fout,true);
				cout<<"\n\n---------------------------------------------------"<<endl;
				cout<<"\n"; 
				fout<<"\n\n";
				system("pause");
				iter=1;
			}
					
			//Se reescribe camino en el primer caso sera el mismo pero para los prox. autos sera diferente
			camino=(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->topeCamino())-1;
			
			usoD= (int)grafo[(SemaforosOrdenados[i].getJ())][camino].getUso(); //uso de la siguiente calle destino (tambien se sobre escribe y sera la misma para el primer caso)
				
			//IMPORTANTE: si calle es AVENIDA se permite mayor paso de autos (en 2xn o 3xn)
			if((grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].getTipoCalle() =="AVENIDA")){nn=3*n;}
				
			//capacidad disponible de calle a cruzar	
			capDisp= (int)(grafo[(SemaforosOrdenados[i].getJ())][camino].getCapacidad())-
			(int)(grafo[(SemaforosOrdenados[i].getJ())][camino].getUso());
				
			if(((grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].getPasoN())< nn) && capDisp > 0){
			//::::::::::::::::::::::IMPORTANTE:::::::::::::::::::
			//Recien desencolo aqui ya q debo verificar antes de ,descartar el camino, si hay capacidad disponible y el semaforo me permite el paso(con pasoN)
				grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->desencolarCamino();//Se desencola camino usado
				//Paso los autos de la "cola" de calle origen a la "cola" de calle siguiente
				grafo[(SemaforosOrdenados[i].getJ())][camino].aumentarUso(1);usoD++;//Se actualiza info de calle q recibe autos
				
				grafo[(SemaforosOrdenados[i].getJ())][camino].agregarAutoEnCalle(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getCopiaAuto(usoO-1),usoD-1);
				
				grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->setAvance(false);
				fout<<"\nAvanza auto con id: "<<grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->getId()<<"\n";
				fout<<"De esquina "<<(SemaforosOrdenados[i].getJ())+1<<" a esquina "<<camino +1<<endl;
				
				if(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->getId()==1)
				{
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x03);
					cout<<"-------------------------------------"<<endl;
					cout<<"\n Avanza auto con id: "<<grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->getId()<<"\n";
					cout<<" De esquina "<<(SemaforosOrdenados[i].getJ())+1<<" a esquina "<<camino +1<<endl;
					cout<<"\n-------------------------------------"<<endl;}
				
				// Actualizo contador de pasos en la calle de autos(como maximo--> calles COMUNES: n && AEVIDAS: 3xn)
				grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].aumentarPasoN(1); paso=true;
				//Se actualiza prioridad del auto en calle nueva
				if(grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->getPrioridad() < 999)
				{
					grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->setPrioridad(0 - usoD + (int)grafo[(SemaforosOrdenados[i].getJ())][camino].getCapacidad());
				}
				if(grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->getPrioridad() >=999)
				{
					grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->setPrioridad(999 - usoD + (int)grafo[(SemaforosOrdenados[i].getJ())][camino].getCapacidad());
				}
				//Actualizacion de ocupacion en calle destino
				grafo[(SemaforosOrdenados[i].getJ())][camino].setOcupacion(
				((grafo[(SemaforosOrdenados[i].getJ())][camino].getUso())/(grafo[(SemaforosOrdenados[i].getJ())][camino].getCapacidad())*100));
				//Se eliminan autos de calle origen
				grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].disminuirUso(1);usoO--; 
				//Actualizacion de ocupacion en calle origen
				grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].setOcupacion(
				((grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].getUso())/(grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].getCapacidad())*100));
					
				//Anuncio de llegada y eliminacion de auto q cumplio su recorrido
				if(grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->caminoVacio()==true)
				{
					llegadas++; llegadas_total++;
					//Escritura en archivo
					fout<<"Ha llegado un ";
					fout<<"Auto con id: "<<grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->getId()
					<<" a su destino: "<<camino+1<<endl;
					
					if(grafo[SemaforosOrdenados[i].getJ()][camino].getAuto(usoD-1)->getId()==1)
					{
						SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x03);
						cout<<"----------------------------------------------"<<endl;
						cout<<" Ha llegado un ";
						cout<<"Auto con id: "<<grafo[(SemaforosOrdenados[i].getJ())][camino].getAuto(usoD-1)->getId()
						<<" a su destino: "<<camino+1<<endl;
						cout<<"----------------------------------------------"<<endl;}
					
					//Eliminacion de auto en calle destino
					if(grafo[(SemaforosOrdenados[i].getJ())][camino].getUso() >0)
					{
						grafo[(SemaforosOrdenados[i].getJ())][camino].disminuirUso(1);usoD--;//Actualizacion de uso calle destino
						//Actualizacion de ocupacion en calle destino
						grafo[(SemaforosOrdenados[i].getJ())][camino].setOcupacion(
						((grafo[(SemaforosOrdenados[i].getJ())][camino].getUso())/(grafo[(SemaforosOrdenados[i].getJ())][camino].getCapacidad())*100));
					}}}
			if(((grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].getPasoN())>= nn)) //&& paso==false)//if Nro.8
			{
				fout<<"\nYa han pasado "<<nn<<" autos por el semaforo "<<(SemaforosOrdenados[i].getI())+1<<" "<<(SemaforosOrdenados[i].getJ())+1<<endl;
				fout<<"Los autos restantes deberan esperar.\n";
				
				if(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->getId()==1)
				{
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x03);
					cout<<"---------------------------------------------"<<endl;
					cout<<"\n Ya han pasado "<<nn<<" autos por el semaforo "<<(SemaforosOrdenados[i].getI())+1<<" "<<(SemaforosOrdenados[i].getJ())+1<<endl;
					cout<<" Los autos restantes deberan esperar.\n";
					cout<<"---------------------------------------------"<<endl;
				}
				usoO=0;
			}
			if(capDisp<=0)
			{
				fout<<"\nCapcidad colmada. Los autos deberan esperar\n";
				if(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->getId()==1)
				{
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x03);
					cout<<"--------------------------------------------"<<endl;
					cout<<"\n Capcidad colmada. Los autos deberan esperar\n";
					cout<<"--------------------------------------------"<<endl;
				}
				grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getAuto(usoO-1)->setAvance(false);
				if(grafo[SemaforosOrdenados[i].getI()][SemaforosOrdenados[i].getJ()].getTipoCalle()=="COMUN"){usoO=0;}	
			}	
			}
				
			else{//Sale del ciclo while para continuar con el for
			usoO=0;
			heapsort(grafo, false);//Reordena las colas antes de salir, para el cruce del prox auto
			}
			}
			
		//Contador de calles vacias
		if(grafo[(SemaforosOrdenados[i].getI())][(SemaforosOrdenados[i].getJ())].getOcupacion()==0){calles_vacias++;}
		heapsort(grafo, false);
}
	
	SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
	cout<<"\n\nCANTIDAD DE AUTOS AGREGADOS: "<<autosAgregados<<endl;
	cout<<"CANTIDAD DE LLEGADAS DE AUTOS POR ITERACION: "<<llegadas<<endl;
	cout<<"CANTIDAD DE LLEGADAS DE AUTOS EN TOTAL: "<<llegadas_total<<endl;
	cout<<"CANTIDAD DE CALLES SIN AUTOS: "<<calles_vacias<<endl;
	cout<<"CANTIDAD DE CALLES CON AUTOS: "<<cantidad_calles-calles_vacias<<endl;
	cout<<"CANTIDAD TOTAL DE CALLES: "<<cantidad_calles<<endl;
	cout<<"CANTIDAD DE AUTOS EN CALLE: "<<CantidadAutos(grafo)<<endl<<endl;
	
	fout<<"\n\nCANTIDAD DE AUTOS AGREGADOS: "<<autosAgregados<<endl;
	fout<<"CANTIDAD DE LLEGADAS DE AUTOS POR ITERACION: "<<llegadas<<endl;
	fout<<"CANTIDAD DE LLEGADAS DE AUTOS EN TOTAL: "<<llegadas_total<<endl;
	fout<<"CANTIDAD DE CALLES SIN AUTOS: "<<calles_vacias<<endl;
	fout<<"CANTIDAD DE CALLES CON AUTOS: "<<cantidad_calles-calles_vacias<<endl;
	fout<<"CANTIDAD TOTAL DE CALLES: "<<cantidad_calles<<endl;
	fout<<"CANTIDAD DE AUTOS EN CALLE: "<<CantidadAutos(grafo)<<endl;
	fout.close();
	heapsort(grafo, false);
	CrearMatrizPeso(grafo);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//                                  PROGRAMA PRINCIPAL

                                                      
int main(){
    
	SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x03);
	cout<<"\n\n                       ALGORITMOS Y ESTRUCTURAS DE DATOS \n "<<endl;
	cout<<"________________________________TRABAJO FINAL________________________________\n\n";
	cout << endl;
	cout << "                               IMPORTANTE                                   " << endl;
	cout << "Siendo la posicion [i][j] en la matriz de referencia, la calle va de i a j" << endl<< endl;
	SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
	
	
	//Lectura del archivo
    int **MA;//Matriz de Adyacencia
    calle **grafo;//grafo
	LeerArchivo(MA,grafo);
    
    // Esta es la funcion que modela nuestra ciudad como MR
    HacerGrafo(grafo, MA);
    
    //cantidad de calles es una variable global
    AuxOrdenarSemaforos SemaforosOrdenados[cantidad_calles];
    //AuxOrdenarSemaforos SemaforosOrdenados[200];
    
    // Imprime el grafo
    ImprimirGrafo(grafo, tamano_ciudad);
    
    //CREo la matriz de peso
    CrearMatrizPeso(grafo);
    
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //                     WHILE PRINCIPAL MENU
    int control=0;
    int idd;
    while(control==0){
			cout << endl;
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x3B); 
            cout << "                                                                               "<<endl;         
			SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x3F); 
			cout << "--------------------------------------MENU-------------------------------------" << endl;
			SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x3B); 
            cout << "                                                                               "<<endl;
			SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0xF3); 
            cout << "               * Agregar mas autos:                            1               " << endl;
            cout << "               * Imprimir Matriz Referencia:                   2               " << endl;
            cout << "               * Cantidad de autos                             3               " << endl;
            cout << "               * Buscar Auto por Id                            4               " << endl;
            cout << "               * Iteracion                                     5               " << endl;
            cout << "               * Ordenar e imprimir semaforos                  6               " << endl;
            cout << "               * Cantidad de Calles                            7               " << endl;
            cout << "               * Ordenar Autos en Calles. Menor a Mayor        8               " << endl;
            cout << "               * Imprimir Matriz de Peso                       9               " << endl;
            cout << "               * Salir:                                       10               " << endl;
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x3B); 
            cout << "                                                                               "<<endl;
            cout << "                                                                               "<<endl;
            
            SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
            cin >> control;
            if(control == 1){
                   cout << "Cuantos autos desea agregar? " << endl;
                   cin >> control;
                   autosAgregados=autosAgregados+control;
                   AgregarAutos(grafo, control);
                   control =0;
                   }
            if(control == 2){
				SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                   ImprimirGrafo(grafo, tamano_ciudad);
                   control =0;
                   }
            if(control ==3){
				SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                       cout << "La cantidad de autos en la ciudad es de: " << CantidadAutos(grafo) << endl;
                       control = 0;
                       }
            if(control ==4){
				SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                       cout << "Recuerde que el auto 'Nuestro' es id = 1. Ingrese el id del Auto a buscar: " << endl;
                       cin >> idd; 
                       control = BuscarAuto(grafo,idd);
                       control = 0;
                       }
             if(control == 5){
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                   cout << "Cuantos Iteraciones desea hacer? " << endl;
                   cin >> control;
                   for(int i=0; i<control; i++){Iteracion(grafo, control, &SemaforosOrdenados[0]);}
                   control =0;
                   }
             if(control == 6){
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                   cout << endl << "Hay " << cantidad_calles << " semaforos en la ciudad" << endl << endl;
                   OrdenarSemaforos(grafo,SemaforosOrdenados,false);
                   cout<<"Los semaforos se han ordenado y se encuentran en Semaforos_Ordenados.txt\n\n";
                   control =0;
                   }
             if(control == 7){
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                   cout << endl << "Hay " << cantidad_calles << " calles en la ciudad" << endl << endl;
                   control =0;
                   }
             if(control == 8){
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                   cout << endl;
                   heapsort(grafo, false);
                   cout<<"Los autos se han ordenado en AutosOrdenados_MenorMayor.txt\n"<<endl;
                   control =0;
                   }                   
             if(control == 9){
					SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
                   cout << endl;
                   CrearMatrizPeso(grafo);
                   control =0;
                   }  
				  SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),0x07);
            }     
	return 1;  
}
