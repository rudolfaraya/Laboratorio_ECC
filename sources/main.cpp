#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

using namespace std;

struct Punto{
   int x;
   int y;
}punto;


//fichero_salida.open(nombre_archivo.c_str());

//-------------------VARIABLES PARA CAMBIO DE CONSOLA--------------------
int c_copiaStdout = dup(1);
int c_consolaActual = 1;
FILE * c_archivoTemporal = fopen("/tmp/asdf.log","w");
//-----------------------------------------------------------------------


//-------------------FUNCIONES PARA CAMBIO DE CONSOLA---------------------
int crearConsola() {

    FILE * f = fopen("/tmp/asdf.log","w");
    fclose(f);
    
    char  pidarg[256]; 
    pid_t hijo;       
    pid_t padre;     
    FILE* fp;         
    int   fn;         

    padre = getpid();
    sprintf( pidarg, "--pid=%d", padre );

    hijo = fork(); 
    if( hijo == 0 ) {

        execlp( "gnome-terminal", "gnome-terminal", "-x", "tail","-f","/tmp/asdf.log", pidarg, NULL );
        perror("execlp()");
        exit( -1 );
    } 

    return 0;
}

void cambiarConsola(int valor) {

    if ( (valor == 1 || valor == 2) && valor != c_consolaActual) {

        if (valor == 2) {

            FILE * fp = c_archivoTemporal;
            int fn = fileno(fp);

            dup2(fn, 1);
        }

        else if (valor == 1) {

            dup2(c_copiaStdout, 1);
        }


        c_consolaActual = valor;
        
    }
}
//--------------------------------------------------------------------------------------------

//----------------------FUNCION PARA CREAR FLUJO DE SALIDA HACIA EL ARCHIVO DE SALIDA---------
int escribirFicheroSalida(string mensaje){
    ofstream fichero_salida("Datos.txt", ios::app);
    
    if(fichero_salida.fail()){
        cout << "No se pudo abrir el archivo de salida Log.txt" << endl;
        return 0; 
    }
    fichero_salida << mensaje;


}


//----------------------FUNCION QUE CALCULA EL MODULO ENTRE DOS VALORES-----------------------
//No se restringe si el primer valor es negativo, calcula uno equivalente
int mod(int x, int y){
    int resultado;

    while(x < 0){
        x = x + y;
    }
    resultado = x % y;
    return resultado;
}

//----------------------FUNCION SUMA ELIPTICA ENTRE DOS PUNTOS--------------------
Punto sumaEliptica(Punto P, Punto Q, int coeficiente_a, int p){
    Punto S;
    int lambda;
    int superior, inferior, k=1;
    int auxiliar, variable;
    char buffer[300];
    
    // Si los Puntos P y Q son iguales
    if(P.x == Q.x && P.y == Q.y && P.y != 0){
        //cout << "Los Puntos P("<< P.x << "," << P.y << ") "<<"y Q("<< Q.x << "," << Q.y << ") "<< "son iguales"<< endl;
        sprintf(buffer,"\nLos Puntos P(%d,%d) y Q(%d,%d) son iguales",P.x,P.y,Q.x,Q.y);
        escribirFicheroSalida(buffer); 

        superior = (3*P.x*P.x + coeficiente_a);
        inferior = (2*P.y);
        //cout << superior << " - " << inferior << endl;
        if(superior%inferior == 0 && superior > 0 && inferior > 0){
            lambda = superior/inferior;
            //cout << "Mi lambda (antes de mod) es: " << lambda << endl;
            lambda = mod(lambda,p);
            //cout << "Mi lambda es: " <<  lambda << endl;
            sprintf(buffer,"\nMi lambda es %d",lambda);
            escribirFicheroSalida(buffer);    
        }else{
            auxiliar = p*k+1;
            //cout << auxiliar << endl; 
            while(mod(auxiliar,inferior) != 0){
                k++;
                auxiliar = (p*k+1);
                //cout << auxiliar << endl;
            }
            variable = auxiliar/inferior;
            lambda = mod((superior*variable),p);
            //cout << "Mi lambda es: " <<  lambda << endl;
            sprintf(buffer,"\nMi lambda es %d",lambda);
            escribirFicheroSalida(buffer);
        }      

        S.x = (lambda*lambda - 2*P.x);
        //cout << "Mi S.x (antes de mod) es: " <<  S.x << endl;
        S.x = mod(S.x,p);
        //cout << "Mi S.x es: " <<  S.x << endl;
        sprintf(buffer,"\nMi S.x es %d",S.x);
        escribirFicheroSalida(buffer);

        S.y = ((lambda)*(P.x - S.x) - P.y);
        //cout << "Mi S.y (antes de mod) es: " <<  S.y << endl;
        S.y = mod(S.y,p);
        //cout << "Mi S.y es: " <<  S.y << endl;
        sprintf(buffer,"\nMi S.y es %d",S.y);
        escribirFicheroSalida(buffer);
        return S;

    // Si los Puntos P y Q son distintos
    }else{
        //cout << " Los Puntos P("<< P.x << "," << P.y << ") "<<"y Q("<< Q.x << "," << Q.y << ") "<< "son distintos"<< endl;
        if(P.x == Q.x){
            S.x = 0;
            //cout << "Mi S.x es: " <<  S.x << endl;
            sprintf(buffer,"\nMi S.x es %d",S.x);
            escribirFicheroSalida(buffer);

            //cout << "Mi S.y (antes de mod) es: " <<  S.y << endl;
            S.y = 0;
            cout << "Mi S.y es: " <<  S.y << endl;
            sprintf(buffer,"\nMi S.y es %d",S.y);
            escribirFicheroSalida(buffer);
            return S;    
        }
        superior = (Q.y - P.y);
        inferior = (Q.x - P.x);
        //cout << superior << " - " << inferior << endl; 
        if(superior%inferior == 0 && superior > 0 && inferior > 0){
            lambda = superior/inferior;
            //cout << "Mi lambda es: (antes de mod)" <<  lambda << endl;
            lambda = mod(lambda,p);
            //cout << "Mi lambda es: " <<  lambda << endl;
            sprintf(buffer,"\nMi lambda es %d",lambda);
            escribirFicheroSalida(buffer);
        }else{
            auxiliar = p*k+1;
            //cout << auxiliar << endl; 
            while(mod(auxiliar,inferior) != 0){
                k++;
                auxiliar = (p*k+1);
                //cout << auxiliar << endl;
            }
            variable = auxiliar/inferior;
            lambda = mod((superior*variable),p);
            //cout << "Mi lambda es: " <<  lambda << endl;
            sprintf(buffer,"\nMi lambda es %d",lambda);
            escribirFicheroSalida(buffer);
        }    
        
        S.x = (lambda*lambda - P.x - Q.x);
        //cout << "Mi S.x (antes de mod) es: " <<  S.x << endl;
        S.x = mod(S.x,p);
        //cout << "Mi S.x es: " <<  S.x << endl;
        sprintf(buffer,"\nMi S.x es %d",S.x);
        escribirFicheroSalida(buffer);
        S.y = ((lambda)*(P.x - S.x) - P.y);
        //cout << "Mi S.y (antes de mod) es: " <<  S.y << endl;
        S.y = mod(S.y,p);
        //cout << "Mi S.y es: " <<  S.y << endl;
        sprintf(buffer,"\nMi S.y es %d",S.y);
        escribirFicheroSalida(buffer);
        return S;
    }    
}

//----------------------FUNCION PRODUCTO ESCALAR ENTRE UN ENTERO "K" Y UN PUNTO P (SUMA ABREVIADA) --------
// k*P = P + P + P +....P (k veces)
Punto productoEscalar(Punto P, int k, int coeficiente_a, int p){
    char buffer[300];
    int contador = 1;
    Punto resultado;
    //ofstream salida("Datos.txt", ios::app);

    if(P.x == 0 && P.y == 0){
        return P;    
    }
    resultado.x = P.x;
    resultado.y = P.y;
    if(k ==1){
        return P;
    }
    if(k > 1){
        while(contador < k){
            resultado = sumaEliptica(resultado,P,coeficiente_a,p);
            contador++;
        }
        //cout << "\nEl resultado del Producto entre k = " << k <<" y el Punto P("<< P.x << "," << P.y << ") es el Punto Resultado("<< resultado.x << "," << resultado.y << ")\n"<<endl; 
        sprintf(buffer,"\nEl resultado del Producto entre k = %d y el Punto P(%d,%d) es el Punto Resultado(%d,%d)\n",k,P.x,P.y,resultado.x,resultado.y);
        escribirFicheroSalida(buffer);
        return resultado;
    }
}

//----------------------FUNCION QUE RESTA DOS PUNTOS (P - Q)------------------------
Punto restaEliptica(Punto P, Punto Q, int coeficiente_a, int p){
    Punto minuendo;
    char buffer[300];
    Punto resultado;
    //ofstream salida("Datos.txt", ios::app);
    // -P (x,y) = P (x,-y)
    minuendo.x = Q.x;
    minuendo.y = -Q.y;

    //cout << "\nEmpieza Resta\n" << endl;
    sprintf(buffer,"\nEmpieza Resta\n");
    escribirFicheroSalida(buffer);

    resultado = sumaEliptica(P,minuendo,coeficiente_a,p);
    return resultado;
}

//----------------------VALIDACION SI EL NUMERO INGRESADO ES PRIMO-------------------
int esPrimo(int numero){
    int divisores = 0; //contador de divisores
    if(numero < 2){
        return 0; //No es primo, debe ser mayor de 1
    }

    for(int i=1;i<=numero;i++){
        if(numero%i == 0){
            divisores++;
        }
    }
    if(divisores == 2){
        return 1; //Es primo
    }else{
        return 0; //No es primo
    }

}

int validarCoeficientes(int a, int b, int p){
    int auxiliar; 
    auxiliar = (4*a*a*a + 27*b*b);
    auxiliar = mod(auxiliar,p);
    //ofstream salida("Datos.txt", ios::app);

    if(auxiliar != 0){
        cout << "\nLos coeficientes a y b son válidos" << endl;
        return 1; //Son validos
    }else{
        cout << "\nLos coeficientes a y b no son correctos. Verifique" << endl;
        return 0; //No son validos
    }

}


//----------------------LISTA TODOS LOS PUNTOS POSIBLES DE LA CURVA ELIPTICA--------
//Guarda el resultado en un vector de Puntos
vector <Punto> listarPuntos(int a, int b, int p){
    char buffer[300];
    /////////////////// LISTAR PUNTOS VALIDOS DE LA CC ///////////////////////////
    vector <Punto> puntos_cc;
    //ofstream salida("Datos.txt", ios::app);
    int matriz [p][5];
    //Llenar matriz de -1
    for(int i=0;i<=p-1;i++){
        for(int j=0;j<5;j++){
            matriz[i][j] = -1;
        }
    }
    /*
    //Imprimir Matriz
    for(int i=0;i<=p-1;i++){
        for(int j=0;j<5;j++){
            cout << "\t|"<<matriz[i][j] << "|\t";
        }
        cout << endl;
    }
    cout << endl;
    */
    //Paso 1 -----------------------------------------------------------
    // (x³  + ax + b)mod p = z 
    //cout << "PASO 1" << "\n"<<endl;
    int z = 0;
    for (int i=0; i <=p-1; i++){
        z = mod((i*i*i + a*i + b),p);
        matriz[i][0] = i;
        matriz[i][1] = z;
    }
    /*
    //Imprimir Matriz
    for(int i=0;i<=p-1;i++){
        for(int j=0;j<5;j++){
            cout << "\t|"<<matriz[i][j] << "|\t";
        }
        cout << endl;
    }
    cout << endl;
    */
    //Paso 2 -----------------------------------------------------------
    // (y²)mod p = z 
    //cout << "PASO 2" << "\n"<<endl;
    z = 0;
    for (int i=0; i <=p-1; i++){
        z = mod((i*i),p);
        matriz[i][2] = z;
    }
    /*
    //Imprimir Matriz
    for(int i=0;i<=p-1;i++){
        for(int j=0;j<5;j++){
            cout << "\t|"<<matriz[i][j] << "|\t";
        }
        cout << endl;
    }
    cout << endl;
    */
    //Paso 3 -----------------------------------------------------------
    // Como z = (y²)mod p  
    //cout << "PASO 3" << "\n"<<endl;
    z = 0;
    for (int i=0; i <=p-1; i++){
        for(int j=0; j<=p-1; j++){
            if(matriz[j][2] == i){
                if(matriz[i][3] == -1){
                    matriz[i][3] = j;    
                }else{
                    matriz[i][4] = j; 
                }                   
            }    
        }
        
    }
    /*
    //Imprimir Matriz
    for(int i=0;i<=p-1;i++){
        for(int j=0;j<5;j++){
            cout << "\t|"<<matriz[i][j] << "|\t";
        }
        cout << endl;
    }
    cout << endl;
    */
    //Paso 4 -----------------------------------------------------------
    // Como z = (x³ + ax + b)mod p  
    //cout << "PASO 4" << "\n"<<endl;
    int d;
    Punto auxiliar;
    for (int i=0; i <=p-1; i++){
        d = matriz[i][1];
        if(matriz[d][3] != -1){
            auxiliar.x = i;
            auxiliar.y = matriz[d][3];
            puntos_cc.push_back(auxiliar);
        }            
        if(matriz[d][4] != -1){
            auxiliar.y = matriz[d][4];
            puntos_cc.push_back(auxiliar);
        }    

    }
    //Imprimir Vector
    
    for(int i=0;i<puntos_cc.size();i++){
     
        cout<<puntos_cc[i].x << "," << puntos_cc[i].y << endl;
    }
    cout << endl;
    
    return puntos_cc;
}

vector <Punto> encriptar(Punto texto_claro, Punto generador, Punto clave_publica, int k, int coeficiente_a, int p){
    char buffer[300];
    //ofstream salida("Datos.txt", ios::app);
    vector <Punto> puntos_cifrados;
    Punto punto_cifrado_1;
    Punto punto_cifrado_2;

    punto_cifrado_1 = productoEscalar(generador, k, coeficiente_a, p);
    Punto sumando2 = productoEscalar(clave_publica, k, coeficiente_a, p);
    punto_cifrado_2 = sumaEliptica(texto_claro, sumando2, coeficiente_a, p);

    puntos_cifrados.push_back(punto_cifrado_1);
    puntos_cifrados.push_back(punto_cifrado_2);
    cambiarConsola(2);
    cout << "\n-------------ENCRIPTACIÓN FINALIZADA---------------\n" << endl;
    sprintf(buffer,"\n-------------ENCRIPTACIÓN FINALIZADA---------------\n");
    escribirFicheroSalida(buffer);    
    //Imprimir Vector
    
    for(int i=0;i<puntos_cifrados.size();i++){
        cout<<puntos_cifrados[i].x << "," << puntos_cifrados[i].y << endl;
        sprintf(buffer,"\n Puntos cifrado: (%d,%d)\n",puntos_cifrados[i].x,puntos_cifrados[i].y);
        escribirFicheroSalida(buffer);    
    }
    cout << endl;
    cout << "\n---------------------------------------------------\n" << endl;

    return puntos_cifrados;
}

Punto desencriptar(Punto punto_cifrado_2, Punto generador, int k, int clave_privada, int coeficiente_a, int p){
    //ofstream salida("Datos.txt", ios::app);
    char buffer[300];
    Punto punto_descifrado;
    Punto minuendo;
    Punto temporal;

    temporal = productoEscalar(generador, k, coeficiente_a, p);
    minuendo = productoEscalar(temporal, clave_privada, coeficiente_a, p);
    punto_descifrado = restaEliptica(punto_cifrado_2, minuendo, coeficiente_a, p);
    cambiarConsola(2);
    cout << "\n-------------DESENCRIPTACIÓN FINALIZADA---------------\n" << endl;
    sprintf(buffer,"\n-------------DESENCRIPTACIÓN FINALIZADA---------------\n");
    escribirFicheroSalida(buffer);  
    //Imprimir Punto Descrifrado
    
    cout<<punto_descifrado.x << "," << punto_descifrado.y << endl;
    cout << endl;
    sprintf(buffer,"\n Puntos descifrado: (%d,%d)\n",punto_descifrado.x,punto_descifrado.y);
    escribirFicheroSalida(buffer);
    cout << "\n---------------------------------------------------\n" << endl;
    return punto_descifrado;
}

int validarG(Punto G, vector <Punto> puntos_ec, int coeficiente_a, int p){
    int numero_puntos = puntos_ec.size();
    //ofstream salida("Datos.txt", ios::app);
    char buffer[300];
    Punto Inverso;
    Punto auxiliar;
    Inverso.x = G.x;
    //Es equivalente a una coordenada negativa, su valor positivo calculado del modulo con su primo p
    Inverso.y = mod(-G.y,p);
    //cout << "\n-------------VALIDACION DE PUNTO GENERADOR G(" << G.x << "," << G.y << ")---------------\n" << endl;
    //cout << "Mi inverso es: (" << Inverso.x << "," << Inverso.y << ")" << endl; 

    sprintf(buffer,"\n-------------VALIDACION DE PUNTO GENERADOR G(%d,%d)---------------\n",G.x,G.y);
    escribirFicheroSalida(buffer);
    sprintf(buffer,"\nMi inverso es: (%d,%d)\n",Inverso.x,Inverso.y);
    escribirFicheroSalida(buffer);
    int n = 3;

    auxiliar = productoEscalar(G,n-1,coeficiente_a,p);
    while(auxiliar.x != Inverso.x || auxiliar.y != Inverso.y){
        n++;
        auxiliar = productoEscalar(auxiliar,n-1,coeficiente_a,p);
        //cout << "Mi auxiliar es: (" << auxiliar.x << "," << auxiliar.y << ") en la iteracion n = " << n << "\n" << endl; 
        sprintf(buffer,"\nMi auxiliar es: (%d,%d) en la iteración n = %d\n",auxiliar.x,auxiliar.y, n);
        escribirFicheroSalida(buffer);
    }
    if(esPrimo(n) == 1){
        //cout << "\nn encontrado es: " << n << endl;
        //cout << " El Punto G("<< G.x << "," << G.y << ") es Punto Generador valido" << endl;
        sprintf(buffer,"\nn encontrado es: %d.\n El Punto G (%d,%d) es Punto Generador Válido\n",n,G.x,G.y);
        escribirFicheroSalida(buffer);
        return 1; //Es valido
    }else{
        //cout << "\nn encontrado no es primo: " << n << endl;
        //cout << " El Punto G("<< G.x << "," << G.y << ") no es Punto Generador valido" << endl;
        sprintf(buffer,"\nn encontrado no es primo: %d.\n El Punto G (%d,%d) no es Punto Generador Válido\n",n,G.x,G.y);
        escribirFicheroSalida(buffer);

        return 0; //No es valido
    }
}

//----------------------IMPRIME EL VECTOR DE PUNTOS VALIDOS PARA SER GENERADOR----------------
vector <Punto> listarG(vector <Punto> puntos_ec, int coeficiente_a, int p){
    vector <Punto> puntos_g_validos;
    char buffer[300];
    //ofstream salida("Datos.txt", ios::app);
    for(int i=0;i<puntos_ec.size();i++){
        cout << puntos_ec[i].x << "," << puntos_ec[i].y << endl;
        //salida<< puntos_ec[i].x << "," << puntos_ec[i].y << endl;
        if(validarG(puntos_ec[i], puntos_ec, coeficiente_a, p) == 1){
            puntos_g_validos.push_back(puntos_ec[i]);
        }
    }
    cout << "\n-------------LISTADO DE PUNTOS GENERADORES--------------\n" << endl;
    //salida<< "\n-------------LISTADO DE PUNTOS GENERADORES--------------\n" << endl;
    for(int i=0;i<puntos_g_validos.size();i++){
        cout << puntos_g_validos[i].x << "," << puntos_g_validos[i].y << endl;
        //salida<< puntos_g_validos[i].x << "," << puntos_g_validos[i].y << endl;
    }
    return puntos_g_validos;
}

int verificarClavePrivada(int clave_privada, vector <Punto> puntos_ec){
    //ofstream salida("Datos.txt", ios::app);
    if(clave_privada <= puntos_ec.size()-1 && clave_privada >= 1){
        cout << "\n-------------CLAVE PRIVADA INGRESADA ES VALIDA--------------\n" << endl;
        return 1;
    }else{
        cout << "\n-------------CLAVE PRIVADA INGRESADA NO ES VALIDA--------------\n" << endl;
        return 0;
    }
}

int menu(){
    //ofstream salida("Datos.txt", ios::app);
    char buffer[1000];
    int a=0, b=0, p=0;
    
    int validador_menu=0;
    //Vector que contiene varias estructuras de tipo Punto
    vector <Punto> puntos_cc;
    Punto neutro; neutro.x = 0; neutro.y = 0; 

    //puntos_cc[0].x = 0; puntos_cc[0].y = 0;

    //Vector que contendra los puntos cifrados de cada encriptacion
    vector <Punto> puntos_cifrados;

    //Vector que contendra los puntos generadores validos de la curva ingresada
    vector <Punto> puntos_g_validos;
    //puntos_g_validos[0].x = 0; puntos_g_validos[0].y = 0; 

    //Punto del texto claro a ingresar
    Punto texto_claro;
    texto_claro.x = 0; texto_claro.y = 0;

    //Clave Publica
    Punto clave_publica;
    clave_publica.x = 0; clave_publica.y = 0;

    //Punto Generador
    Punto generador;

    //Numero aleatorio para encriptar y desencriptar
    int k;

    //Clave Privada
    int clave_privada;

    //Punto Descrifado
    Punto punto_descifrado;

    //para validar si ya se ha encriptado
    int ya_encripte=0;;
    int opcion = 0;
    cambiarConsola(1);
    printf("\n\t\t\tCRIPTOGRAFIA DE CURVA ELÍPTICA (ECC)\n\n");
    sprintf(buffer,"\n\t\t\tCRIPTOGRAFIA DE CURVA ELÍPTICA (ECC)\n\n");
    escribirFicheroSalida(buffer);

    //fichero_salida << "\n\t\t\tCRIPTOGRAFIA DE CURVA ELÍPTICA (ECC)\n\n";
    
    while(opcion!=6){      /* "Inicio de Mientras de las opciones del menú" */
            cambiarConsola(1);
            system("clear");
            printf("\nElija que operación desea realizar\n\n");
            printf("0. Ingresar parámetros de la Curva Eliptica\n");
            printf("1. Mostrar Puntos Válidos de la Curva\n");
            printf("2. Mostrar Puntos Válidos Generadores\n");
            printf("3. Encriptar\n");
            printf("4. Desencriptar\n");
            printf("5. Calcular Clave Pública\n");
            printf("6. Salir del Programa\n\n");
            printf("Digite su opción: \n");
            cin >> opcion;
            if(opcion==0) {
                cambiarConsola(2);
                system("clear");
                cambiarConsola(1);
                printf("\n\t\t\tINGRESO DE PARAMETROS DE LA CURVA\n");
                //fichero_salida << "\n\t\t\tINGRESO DE PARAMETROS DE LA CURVA\n";
                cout << "Ingrese a: "<< endl;
                cin >> a;
                cout << "Ingrese b: "<< endl;
                cin >> b;
                cout << "Ingrese p: "<< endl;
                cin >> p;
                cambiarConsola(2);
                int verificar_primo = esPrimo(p);
                if(verificar_primo == 0){
                    cout << "El valor ingresado no es primo, verifique." << endl;
                    menu();
                }
                int verificar_coeficientes = validarCoeficientes(a,b,p);
                    if(verificar_coeficientes == 0){
                    cout << "El valor ingresado no es correcto, verifique." << endl;
                    menu();
                }               
                system("clear");
                printf("\n\t\t\t LA CURVA INGRESADA ES LA SIGUIENTE\n"); 
                sprintf(buffer,"\n\t\t\tLA CURVA INGRESADA ES LA SIGUIENTE\n\n");
                escribirFicheroSalida(buffer);
                validador_menu++;
                cout << "Su curva eliptica es: " << "y² = (x³ + " << a << "x + " << b << ")mod" << p << "\n"<<endl;  
                sprintf(buffer,"\n\t\t\tSu curva eliptica es: y² = (x³ + %dx + %d)mod%d\n",a,b,p);
                escribirFicheroSalida(buffer);
                cambiarConsola(1);
                system("clear");
            }else{ //else 0
            if(opcion==1){
                if(validador_menu == 0){
                    cambiarConsola(2);
                    cout << "No se han ingresado los parámetros para definir la curva" << endl;
                    menu();
                }
                validador_menu++;
                cambiarConsola(2);
                printf("\n\t\t\tLISTADO DE PUNTOS VALIDOS PARA LA CURVA\n");
                sprintf(buffer,"\n\t\t\tLISTADO DE PUNTOS VALIDOS PARA LA CURVA\n\n");
                escribirFicheroSalida(buffer);
                puntos_cc = listarPuntos(a,b,p);
                cambiarConsola(1);
                system("clear");
            }else{ //else 1
            if(opcion==2){
                if(validador_menu == 0){
                    cambiarConsola(2);
                    cout << "No se han ingresado los parámetros para definir la curva" << endl;
                    menu();
                }
                if(validador_menu == 1){
                    cambiarConsola(2);
                    cout << "No se han generado los puntos válidos la curva. Ingrese a la opción 0" << endl;
                    menu();
                }
                cambiarConsola(2);
                validador_menu++;

                printf("\n\t\t\tLISTADO DE PUNTOS VALIDOS GENERADORES LA CURVA\n");
                sprintf(buffer,"\n\t\t\tLISTADO DE PUNTOS VALIDOS GENERADORES PARA LA CURVA\n\n");
                escribirFicheroSalida(buffer);
                puntos_g_validos = listarG(puntos_cc,a,p);
                cambiarConsola(1);
                system("clear");
            }else{ //else 2
            if(opcion==3){
                system("clear"); 
                cambiarConsola(1);
                printf("\n\t\t\tENCRIPTACION EN CURVA ELIPTICA\n");
                sprintf(buffer,"\n\t\t\tENCRIPTACION EN CURVA ELIPTICA\n\n");
                escribirFicheroSalida(buffer);
                cout << "Ingrese el Punto a encriptar:" << endl;
                cout << "Ingrese primera coordenada: ";
                cin >> texto_claro.x;
                cout << "\nIngrese segunda coordenada: ";
                cin >> texto_claro.y;
                cambiarConsola(2);
                system("clear");
                printf("\n\t\t\tENCRIPTACION EN CURVA ELIPTICA\n");
                cout << "Su Punto a encriptar es: ("<< texto_claro.x << "," << texto_claro.y << ") "<< endl;
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tENCRIPTACION EN CURVA ELIPTICA\n");
                cout << "Ingrese la Clave Pública:" << endl;
                cout << "Ingrese primera coordenada: ";
                cin >> clave_publica.x;
                cout << "\nIngrese segunda coordenada: ";
                cin >> clave_publica.y;
                cambiarConsola(2);
                cout << "Su clave_publica es: ("<< clave_publica.x << "," << clave_publica.y << ") "<< endl;                
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tENCRIPTACION EN CURVA ELIPTICA\n");
                cout << "Ingrese Punto Generador:" << endl;
                cout << "Ingrese primera coordenada: ";
                cin >> generador.x;
                cout << "\nIngrese segunda coordenada: ";
                cin >> generador.y;
                cambiarConsola(2);
                cout << "Su Punto Generador es: ("<< generador.x << "," << generador.y << ") "<< endl;
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tENCRIPTACION EN CURVA ELIPTICA\n");
                cout << "Ingrese numero k, necesario para encriptar:" << endl;
                cin >> k;
                cambiarConsola(2);
                cout << "Su numero k es: "<< k << endl;
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tENCRIPTACION EN CURVA ELIPTICA\n");
                ya_encripte = 1;
                puntos_cifrados = encriptar(texto_claro, generador, clave_publica, k, a, p);
                cambiarConsola(2);

            }else{
            if(opcion==4){
                if(ya_encripte == 0){
                    cambiarConsola(2);
                    validador_menu++;
                    cout << "No hay nada que desencriptar. Realice una encriptación primero." << endl;
                    menu();
                }
                cambiarConsola(2);
                system("clear");
                printf("\n\t\t\tDESENCRIPTACION EN CURVA ELIPTICA\n");
                sprintf(buffer,"\n\t\t\tDESENCRIPTACION EN CURVA ELIPTICA\n\n");
                escribirFicheroSalida(buffer); 
                cout << "Para la desencriptación se necesitan algunos de los valores ingresados para la encriptación" <<endl;
                cout << "Su Punto Generador es: ("<< generador.x << "," << generador.y << ") "<< endl;
                cout << "Su numero k es: "<< k << endl;
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tDESENCRIPTACION EN CURVA ELIPTICA\n");
                cout << "Ingrese la Clave Privada:" << endl;
                cin >> clave_privada;
                cambiarConsola(2);
                int verificar_clave_privada = verificarClavePrivada(clave_privada,puntos_cc);
                if(verificar_clave_privada == 0){
                    cout << "El valor ingresado no se encuentra entre los límites permitidos ["<< 1 << ","<< puntos_cc.size()-1 << "]"<< endl;
                    menu();
                }
                cout << "Su clave privada es: "<< clave_privada << endl;
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tDESENCRIPTACION EN CURVA ELIPTICA\n");
                punto_descifrado = desencriptar(puntos_cifrados[1], generador, k, clave_privada, a, p);
                cambiarConsola(2);              
            }else{
            if(opcion==5){
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tGENERACIÓN DE CLAVE PÚBLICA\n");
                sprintf(buffer,"\n\t\t\tGENERACIÓN DE CLAVE PÚBLICA\n\n");
                escribirFicheroSalida(buffer); 
                cout << "Recuerde que puede consultar los puntos generadores con la opción 2";
                cout << "Ingrese Punto Generador:" << endl;
                cout << "Ingrese primera coordenada: ";
                cin >> generador.x;
                cout << "\nIngrese segunda coordenada: ";
                cin >> generador.y;
                cambiarConsola(2);
                system("clear");
                printf("\n\t\t\tGENERACIÓN DE CLAVE PÚBLICA\n");
                cout << "Su Punto Generador es: ("<< generador.x << "," << generador.y << ") "<< endl;
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tGENERACIÓN DE CLAVE PÚBLICA\n");
                cout << "Ingrese la Clave Privada:" << endl;
                cin >> clave_privada;
                cambiarConsola(2);
                int verificar_clave_privada = verificarClavePrivada(clave_privada,puntos_cc);
                if(verificar_clave_privada == 0){
                    cout << "El valor ingresado no se encuentra entre los límites permitidos ["<< 1 << ","<< puntos_cc.size()-1 << "]"<< endl;
                    menu();
                }
                cambiarConsola(2);
                cout << "Su clave privada es: "<< clave_privada << endl;
                clave_publica = productoEscalar(generador,clave_privada,a,p);
                system("clear");
                printf("\n\t\t\tGENERACIÓN DE CLAVE PÚBLICA\n");
                cout << "Su clave_publica es: ("<< clave_publica.x << "," << clave_publica.y << ") "<< endl;                
                cambiarConsola(1);
                system("clear");
                printf("\n\t\t\tGENERACIÓN DE CLAVE PÚBLICA\n");

            }

            }//Fin else 5
            }//Fin else 4    
            }//Fin else 2
            }//Fin else 1
            }//Fin else 0

    }    
}


int main(){
    crearConsola();  // Esto tiene que ir al principio del "main".

    //ofstream salida("Datos.txt", ios::app);
    //ofstream salida("log");
    //fichero_salida.open(nombre_archivo.c_str());
    menu();    
    Punto Q;
    Q.x = 6;
    Q.y = 9;
    Punto P;
    P.x = 10;
    P.y = 2;
    //Punto S = sumaEliptica(P,Q,a,p);
    //Punto S = productoEscalar(P,7,a,p);    
    //Punto S = restaEliptica(P,Q,a,p);
    

    
    
    //puntos_g_validos = listarG(puntos_cc, a, p);
    //validarG(Q,puntos_cc,a,p);
    /*
    Punto texto_claro; texto_claro.x = 10; texto_claro.y = 9;
    Punto generador; generador.x = 2; generador.y = 7; 
    Punto clave_publica; clave_publica.x = 7; clave_publica.y = 2;
    int k = 3;
    int clave_privada = 7;

    puntos_cifrados = encriptar(texto_claro, generador, clave_publica, k, a, p);
    
    desencriptar(puntos_cifrados[1], generador, k, clave_privada, a, p);
    */

    //puntos_cc = listarPuntos(a,b,p);
    

    return 0;


}
