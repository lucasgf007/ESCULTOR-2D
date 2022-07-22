#include "sculptor.h"
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "vector"
#include "mainwindow.h"

using namespace std;
//Alocação da Matriz 3D.
Sculptor::Sculptor(int _nx,int _ny,int _nz){
    nl=_nx;nc=_ny;np=_nz;
    if(nl<=0||nc<=0||np<=0){
        nl=nc=np=0;
    }
    v=new Voxel**[nl];
    if(v==nullptr){
        cout<<"Erro na alocação."<<endl;
        exit(0);
    }
    v[0]=new Voxel*[nl*nc];
    if(v[0]==nullptr){
        cout<<"Erro na alocação."<<endl;
        exit(0);
    }
    v[0][0]=new Voxel[nl*nc*np];
    if(v[0][0]==nullptr){
        cout<<"Erro na alocação."<<endl;
        exit(0);
    }
    for(int i=0; i< nl; i++)
    {
        if (i<(nl-1))
        {
            v[i+1] = v[i]+nc;
        }
        for(int j=1; j< nc; j++)
        {
            v[i][j] = v[i][j-1]+np;
            if(j==nc-1 && i!=(nl-1))
            {
                v[i+1][0] = v[i][j]+np;
            }
        }
    }
    for(int i = 0; i < nl; i++){
        for(int j = 0; j < nc; j++){
            for(int k = 0; k < np; k++){
                v[i][j][k].isOn = false;
            }
        }
    }
}
//Liberar a memória utilizada.
Sculptor::~Sculptor(){
    if(nl==0||nc==0||np==0){
        return;
    }
    delete [] v[0][0];
    delete [] v[0];
    delete [] v;
}
//Atribuir as cores.
void Sculptor::setColor(int _r, int _g, int _b, float alpha){
    //Verificação das cores utilizadas.
    if((_r<=(255)||_r>=0)||(_g<=255||_g>=0)||(_b<=255||_b>=0)||(alpha<=1||alpha>=0)){
        r=_r;g=_g;b=_b;a=alpha;
    }
    else {
        cout<<"Cores invalidas"<<endl;
        exit(1);
    }
}
//Criar um Voxel.
void Sculptor::putVoxel(int x,int y,int z){
    //Verificação das dimensões atribuida.
    if(x<0 || x>nl || y<0 || y>nc || z<0 || z>np){
        cout <<"Dimensoes erradas, digite novamente"<<endl;
        exit(0);
    }
    //Atribuições das cores para o Voxel selecionado.
    else{
        v[x][y][z].isOn=true;
        v[x][y][z].red=r;
        v[x][y][z].blue=b;
        v[x][y][z].green=g;
        v[x][y][z].alpha=a;
    }
}
//Cortar um Voxel.
void Sculptor::cutVoxel(int x,int y,int z){
    //Verificação das dimensões atribuida.
    if(x<0 || x>nl || y<0 || y>nc || z<0 || z>np){
        cout <<"Dimensoes erradas, digite novamente"<<endl;
        exit(1);
    }
    //Utilizando o isOn para efetuar o corte.
    else{
        v[x][y][z].isOn=false;
    }
}
void Sculptor::writeOFF(string filename){
    //Variavel para efetuar as funções de fluxos.
    ofstream fout2;
    //Contador para o número de vertices.
    int Nvertices=0;
    //Contador para o número de faces.
    int Nfaces=0;
    //Variavel auxiliar na execução e determinação de cada face para cada Voxel.
    int aux=0;
    //Abrindo arquivo.
    fout2.open("C:/Users/JeffersonMelo/Downloads/Sculptor-3D-3.0-master/Sculptor-3D-3.0-master/"+filename+".off");
    //Condição para determinar se o arquivo foi aberto.
    if(fout2.is_open()){
        cout << "arquivo OFF foi aberto\n";
    }
    else{
        cout << "arquivo OFF nao foi aberto\n";
        exit(1);
    }
    //Primeira linha do arquivo. Linha de reconhecimento.
    fout2<<"OFF"<<endl;
    //Contar a quantidade de vertices e faces. Obs.: isOn deve ser verdadeiro.
    for(int i=0;i<nl;i++){
        for(int j=0;j<nc;j++){
            for(int k=0;k<np;k++){
                if(v[i][j][k].isOn){
                    Nvertices=Nvertices+8;
                    Nfaces=Nfaces+6;
                }
            }
        }
    }
    //Segunda linha do arquivo. Linha para mostra quantidade de vertices e faces.
    fout2<<Nvertices<<" "<<Nfaces<<" "<<0<<endl;
    //Dimensionamento de cada vertice em relação a cada Voxel com isOn true. Obs.: dimensão de cada Voxel 1.
    for(int i=0;i<nl;i++){
        for(int j=0;j<nc;j++){
            for(int k=0;k<np;k++){
                if(v[i][j][k].isOn){
                        fout2<<i-0.5<<" "<<j+0.5<<" "<<k-0.5<<endl;
                        fout2<<i-0.5<<" "<<j-0.5<<" "<<k-0.5<<endl;
                        fout2<<i+0.5<<" "<<j-0.5<<" "<<k-0.5<<endl;
                        fout2<<i+0.5<<" "<<j+0.5<<" "<<k-0.5<<endl;
                        fout2<<i-0.5<<" "<<j+0.5<<" "<<k+0.5<<endl;
                        fout2<<i-0.5<<" "<<j-0.5<<" "<<k+0.5<<endl;
                        fout2<<i+0.5<<" "<<j-0.5<<" "<<k+0.5<<endl;
                        fout2<<i+0.5<<" "<<j+0.5<<" "<<k+0.5<<endl;
                }
            }
        }
    }
    //Dimensionar cada face de cada Voxel e atribuindo as cores correspondente.
    for(int i=0;i<nl;i++){
        for(int j=0;j<nc;j++){
            for(int k=0;k<np;k++){
                if(v[i][j][k].isOn){
                    fout2<<4<<" "<<aux+0<<" "<<aux+3<<" "<<aux+2<<" "<<aux+1<<" "<<v[i][j][k].red<<" "
                    <<v[i][j][k].green<<" "<<v[i][j][k].blue<<" "<<v[i][j][k].alpha<<endl;
                    fout2<<4<<" "<<aux+4<<" "<<aux+5<<" "<<aux+6<<" "<<aux+7<<" "<<v[i][j][k].red<<" "
                    <<v[i][j][k].green<<" "<<v[i][j][k].blue<<" "<<v[i][j][k].alpha<<endl;
                    fout2<<4<<" "<<aux+0<<" "<<aux+1<<" "<<aux+5<<" "<<aux+4<<" "<<v[i][j][k].red<<" "
                    <<v[i][j][k].green<<" "<<v[i][j][k].blue<<" "<<v[i][j][k].alpha<<endl;
                    fout2<<4<<" "<<aux+0<<" "<<aux+4<<" "<<aux+7<<" "<<aux+3<<" "<<v[i][j][k].red<<" "
                    <<v[i][j][k].green<<" "<<v[i][j][k].blue<<" "<<v[i][j][k].alpha<<endl;
                    fout2<<4<<" "<<aux+3<<" "<<aux+7<<" "<<aux+6<<" "<<aux+2<<" "<<v[i][j][k].red<<" "
                    <<v[i][j][k].green<<" "<<v[i][j][k].blue<<" "<<v[i][j][k].alpha<<endl;
                    fout2<<4<<" "<<aux+1<<" "<<aux+2<<" "<<aux+6<<" "<<aux+5<<" "<<v[i][j][k].red<<" "
                    <<v[i][j][k].green<<" "<<v[i][j][k].blue<<" "<<v[i][j][k].alpha<<endl;
                    aux=aux+8;
                }
            }
        }
    }
    //Condição para apresentar o fim do arquivo.
    if(fout2.is_open()){
        cout << "arquivo OFF salvo"<<endl;
    }

}
vector<vector<Voxel>> Sculptor::readPlano(int _dim, int _plano){
    vector<vector<Voxel>> matriz;
    vector<Voxel> vetor;
    if(_plano == XY){
        vetor.resize(nc);
        for(int i = 0; i<nl; i++){
            for(int j = 0; j<nc; j++){
                vetor[j].isOn=v[i][j][_dim].isOn;
                vetor[j].red =v[i][j][_dim].red;
                vetor[j].green=v[i][j][_dim].green;
                vetor[j].blue=v[i][j][_dim].blue;
                vetor[j].alpha= v[i][j][_dim].alpha;
            }
            matriz.push_back(vetor);
        }
    }
    if(_plano == ZX){
        vetor.resize(nl);
        for(int i = 0; i<nl; i++){
            for(int j = 0; j <np; j++){
                vetor[j].isOn=v[i][_dim][j].isOn;
                vetor[j].red=v[i][_dim][j].red;
                vetor[j].green=v[i][_dim][j].green;
                vetor[j].blue=v[i][_dim][j].blue;
                vetor[j].alpha=v[i][_dim][j].alpha;
            }
            matriz.push_back(vetor);
        }
    }
    if(_plano == YZ){
        vetor.resize(np);
        for(int i = 0; i<nc; i++){
            for(int j = 0; j<np; j++){
                vetor[j].isOn=v[_dim][i][j].isOn;
                vetor[j].red=v[_dim][i][j].red;
                vetor[j].green=v[_dim][i][j].green;
                vetor[j].blue=v[_dim][i][j].blue;
                vetor[j].alpha=v[_dim][i][j].alpha;
            }
            matriz.push_back(vetor);
        }
    }
    vetor.clear();
    return matriz;
}
