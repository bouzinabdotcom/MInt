/**
 * MInt: Multi-precision Integer
 * 
 * MINI PROJET Realise par:
 * 
 *      Bouzinab Omar
 *      Hammou Tahra Nore
 * 
 * 
 *      Groupe: 3IIR3
 * 
 * 30 Decembre 2019
 *  
**/

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/**
 * La classe DInt represente 2 chiffre dans une case de type char en BCD
**/
class DInt {
    unsigned char val;
    public:
    // Constructeurs:
        DInt(unsigned int iVal = 0){
            if(iVal > 99 || iVal < 0)
            {
                cout << "Erreur: Les valeurs negatives (" << iVal << ") ne sont pas supportees" << endl;
                exit(1);
            }
            val = ((iVal/10) << 4) | (iVal%10); 
        }

        DInt(const char *iVal) {
            if(strlen(iVal) > 2 || *iVal < '0' || *iVal > '9' || *(iVal+1) < '0' || *(iVal+1) > '9')
            {
                cout << "Erreur: Les valeurs negatives (" << iVal << ") ne sont pas supportees" << endl;
                exit(1);
            }
            else if(strlen(iVal) == 1)
                val = ((*iVal - '0') & 0x0F);

                
            else
                val = (((*iVal - '0') << 4) & 0xF0) | ((*(iVal+1) - '0') & 0x0F);
            
        }

    // Operateurs

        DInt &operator=(const DInt &Originale){
            val = Originale.val;
            return *this;
        }

        DInt &operator=(unsigned int n){
            if(n > 99 || n < 0)
            {
                cout << "Erreur: Les valeurs negatives (" << n << ") ne sont pas supportees" << endl;
                exit(1);
            }
            val = ((n/10) << 4) | (n%10); 
            return *this;
        }

        DInt operator+(DInt rhs) {
            return DInt((toInt()+rhs.toInt())%100);
        }

        DInt operator+=(DInt rhs) {
            return *this = *this + rhs;
        }

        bool operator==(DInt rhs){
            return val == rhs.val;
        }

        bool operator>(DInt rhs){
            return val > rhs.val;
        }

        bool operator<(DInt rhs){
            return val < rhs.val;
        }

        bool operator<=(DInt rhs){
            return *this < rhs || *this == rhs;
        }
        bool operator>=(DInt rhs){
            return *this > rhs || *this == rhs;
        }


    // MISC.
        int toInt(){
            char a= (val &  0x0F),
                 b= (val & 0xF0) >> 4;
            return b*10 + a;
        }

        void print(int n = 2) {
       
            if(n==2)
                printf("%x%x", (val&0xF0)>>4, val&0x0F);
            else if(n==1)
                printf("%x", val&0x0F);

        }

        DInt reste(DInt rhs) {
            return DInt((toInt()+rhs.toInt())/100);
        }
        

};


/**
 * La classe MInt est un regroupement de plusieurs objets DInt
 * 
*/
class MInt {
    DInt* MI;
    int nlen;

    MInt* cat(DInt tail) {
        /**
         * Fonction necessaire pour ajouter le reste de l'addition
         * avant d'ajouter le Couple DInt suivant.
         * Privee 
        */
        DInt* nMI = new DInt[nlen+1];
        for(int i=nlen-1; i>=0; i--){
            nMI[i+1]=MI[i];
        }
        nMI[0] = tail;
        nlen++;
        delete[] MI;
        MI = nMI;
        return this;
    }
    
    public:

    // Constructeurs:
        MInt(unsigned int n) {
            int ncp = n, len=1, dec = 100;
            while(ncp/=10)
                len++;
            
            nlen = len / 2 + len % 2; 
            MI = new DInt[nlen];
            
            ncp = n;
            for(int i = nlen - 1; i >= 0; i--) {
                MI[i] = DInt(ncp%dec);
                ncp /= 100;
                
            }
            
        }

        MInt(const char *n = "") {
            
            nlen = strlen(n) / 2 + strlen(n) % 2;
            
            MI = new DInt[nlen];
            char tmp[3] = "00" ;
            for(int i = nlen - 1, j = strlen(n) - 1; i >= 0; i--, j-=2) {
                tmp[0] = j ? n[j-1]:'0' ;
                tmp[1] = n[j];
                MI[i] = DInt(tmp);
            }
            
        }
    // Constructeur de copie:
        MInt(const MInt &Originale) {
            delete[] MI;
            nlen = Originale.nlen;
            MI = new DInt[nlen];
            for(int i = 0; i < nlen; i++){
                MI[i] = Originale.MI[i];
            }
        } 

    // Destructeur: 
        ~MInt(){
            delete[] MI;
        }

       

        MInt &operator=(const MInt &Originale) {
            if(this != &Originale) {
                delete[] MI;
                nlen = Originale.nlen;
                MI = new DInt[nlen];
                for(int i = 0; i < nlen; i++)
                    MI[i] = Originale.MI[i];
            }

            return *this;
        }


    // Operateurs: 

        MInt &operator=(unsigned int n) {
            return *this = MInt(n);
        }


        bool operator==(MInt rhs) {
            if(nlen != rhs.nlen) return false;
            int i;
            for(i=0; i<nlen; i++)
                if(!(MI[i]==rhs.MI[i]))
                    return false;


            return true;
        }


        bool operator<(MInt rhs) {

            if(nlen > rhs.nlen) return false;
            if(nlen < rhs.nlen) return true;
            int i;
            for(i=0; i<nlen; i++){ 
                if(MI[i] < rhs.MI[i]) return true;
                if(MI[i] > rhs.MI[i]) return false;
            }
            

            return false;
        }

        bool operator<=(MInt rhs){
            return *this < rhs || *this == rhs;
        }

        bool operator>(MInt rhs) {
            return !(*this <= rhs );
        }

        bool operator>=(MInt rhs) {
            return *this > rhs || *this == rhs;
        }
        MInt operator+(MInt rhs) {
            MInt resultat = (unsigned int)0 ;
            int i, j, k;
            for(i=nlen-1, j=rhs.nlen-1; i >= 0 && j>=0; i--, j--) {
                resultat.MI[0] += MI[i] + rhs.MI[j];
                
                resultat.cat(MI[i].reste(rhs.MI[j]));
            }

            
            DInt rest;
            if( i>j ) {
                for(; i >= 0; i--){
                    rest = resultat.MI[0].reste(MI[i]);
                    resultat.MI[0] += MI[i];
                    if(!(i == 0 && rest.toInt() == 0)) resultat.cat(rest);
                }
                    
                
            }else if( j>i ) {
                for(; j >= 0; j--){

                    rest = resultat.MI[0].reste(rhs.MI[j]);

                    
                    resultat.MI[0] += rhs.MI[j];
                    
                    if(!(j == 0 && rest.toInt() == 0)) resultat.cat(rest);
                }
            }

            
            
            return resultat;
        }

        MInt operator+=(MInt rhs) {
            return *this = *this + rhs;
        }
    // Operateurs de conversion:
        operator unsigned int(){
            unsigned int val = 0;
            for(int i = 0; i < nlen; i++){
                val *= 100;
                val += MI[i].toInt();
            }
            return val;
        }
        operator double(){
            double val = 0.0;
            for(int i = 0; i < nlen; i++){
                val *= 100;
                val += (double)MI[i].toInt();
            }

            return val;
        }
        
    // MISC.
        void print() {
            for(int i = 0; i < nlen ; i++){
                if(i == 0 && MI[i].toInt() < 10)
                    MI[i].print(1);
                else
                    MI[i].print();

            }
            
        }



};



int main() {


    MInt  r="-1", a="9999999999999999999999999999999999999999999999999999999999999999999999";
   
    
    
    a.print();
    cout << endl << "+" << endl;
    r.print();
    cout << endl << "=" << endl;

    (a+r).print();





    return 0;
}
