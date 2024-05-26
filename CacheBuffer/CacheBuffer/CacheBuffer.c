#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
//@brief double linked listin nodeu
typedef struct Node{
	char* add; //sayfa adresi
	int count; //istek sayisi(sayac)
	struct Node *prev;
    struct Node *next;
}NODE;

//@brief yeni node olusturma
///@return olusturulan yeni node
NODE* createNode(const char* add){
	NODE *newNode;
	newNode=(NODE*)malloc(sizeof(NODE));//dinamik bellekle yer acma
	if (newNode == NULL) {
        printf("Allocation error\n");
        exit(0);
    }
    newNode->add= strdup(add); //strdup ile add kopyalanýr
    newNode->count=1;
    newNode->prev=NULL;
    newNode->next=NULL;
    return newNode;
}

//@brief dizinin basýna node ekleme
void insertionAtFront(NODE **headNode, NODE *newNode){
	if(*headNode==NULL){
		*headNode=newNode;
	}
	else{
	 newNode->next=*headNode;
	 (*headNode)->prev=newNode;
	 *headNode=newNode;	
	}
}
//@brief diziye node eklemek icin node durumunu kontrol ediyor
//@param key kullanýcýdan gelen sayfa adresi
//@param T kullanýcýdan alýnan esik deger
//@param L kullanýcýdan alýnan cache buffer kapasitesi
void searchNode(NODE **headNode, const char* key,int T,int L){
	NODE *tmp;//listeyi dolasmak icin kullanýlýr
	int i=0;
	int flag=0; //donguden cikmak icin
	int count=0; //kapasite kontrolu icin
	if(*headNode==NULL){ 
		tmp=createNode(key);
		insertionAtFront(headNode,tmp);
		printf("\n%s geldi yeni dugum head node atandý\n",tmp->add);
	}
	
	else{
		tmp=*headNode;
		while(tmp!=NULL&&flag==0){
			//dizide daha once bu adres var mi kontrolu
			if(strcmp(tmp->add, key) == 0){
			tmp->count++; //sayac arttirildi
			flag=1; //adres bulundu donguden cýkmak icin
			printf("\n%s geldi sayac arttirildi\n",tmp->add);
			//sayac arttirildiktan sonra esik deger kontrol ediliyor
		    	if(tmp != *headNode && tmp->count > T)//esik deger gecildiyse node bas node yapiliyor
		       	{  
		       	   
			       if (tmp->prev != NULL) {
                        tmp->prev->next = tmp->next;
                    }
                    if (tmp->next != NULL) {
                        tmp->next->prev = tmp->prev;
                    }
                    tmp->next = *headNode;
                    if (*headNode != NULL) {
                        (*headNode)->prev = tmp;
                    }
                    tmp->prev = NULL;
                    *headNode = tmp;
                    printf("[T=%d],%s %d head node yapildi\n", T,tmp->add,tmp->count);
			}
		}  
		//listede bakýlan node key ile eslesmedi listenin sonraki elemanýna bakmak iöin
			else{
		    tmp=tmp->next; //bir sonraki node'a gecilir	
			}
		}
		if(flag==0){
		//donguden liste bittigi için cikildi adres bulunamadý adres listeye yeni node olarak ekleniyor
		tmp=createNode(key);
		insertionAtFront(headNode,tmp);
			
		printf("\n%s geldi yeni dugum head node atandý\n",tmp->add);
		count=countNodes(headNode);
		
		//kapasite kontrolu
		if(count>L)
	    { 
		  deleteAtEnd(headNode);
		  printf("[L=%d] son(%d.)dugum silindi\n",L,count);
		 }
     	}
	}
	 
}
//@brief kapasiteyi kontrol etmek icin dizideki nodelarý sayma
//@return liste uzunlugu
 int countNodes(NODE **headNode) {
    NODE *tmp ;
	tmp=*headNode;
    int count = 0;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

//@brief kapasiteyi asýnca veya kullanici listeyi silmek istediginde sondan eleman silme
void deleteAtEnd(NODE **headNode){
	NODE *tmp;
	//dizi bos mu kontrolu 
	if(*headNode==NULL){
		printf("Empty List\n");
	}
	//dizide tek eleman varsa
	else if((*headNode)->next==NULL)
	{
		*headNode=NULL;
		free(*headNode);
	}
	//dizideki son elemana ulasma
	else{
	tmp=*headNode;	
	while(tmp->next!=NULL){
		tmp=tmp->next;
	  }
     tmp->prev->next=NULL;
     free(tmp);
	}
	
}
//@brief listeyi yazdýrma fonskiyonu
void printList(NODE **headNode){
	NODE *tmp;
	tmp=*headNode;
	if(tmp==NULL){
		printf("Empty list\n");
	}
	while(tmp->next!=NULL){
		printf("%s,%d -><- ",tmp->add,tmp->count);
		tmp=tmp->next;
	}
	printf("%s,%d ",tmp->add,tmp->count);
	
}



int main(int argc, char *argv[]) {
	NODE *headNode;
	headNode=NULL;
	int choice; //listeyi dosyadan veya kullanicidan alma ve listeyi silmek isteme tercihi 
	int T,L;
	int flag=0;//donguden cýkmak icin
	char dosyaAdi[MAX_LENGTH]; // Maksimum dosya adý uzunluðu 100
    FILE* dosya;
    char satir[MAX_LENGTH]; 
    printf("Istek yapilan sayfalar\n");
    printf("Dosyadan okuma icin 1'e basin\n");
    printf("Kullanicidan alma icin 2'ye basin\n");
    scanf("%d",&choice);
    printf("Esik degerini girin\n");
	scanf("%d",&T);
	printf("Cache buffer kapasite degerini girin\n");
	scanf("%d",&L);
    if(choice==1){
    	printf("Dosya adini giriniz: ");
    scanf("%s", dosyaAdi);
     // Dosyayý okuma modunda aç
    dosya = fopen(dosyaAdi, "r");

    // Dosya açýldý mý kontrol et
    if (dosya == NULL) {
        printf("Dosya acilamadi.");
        return 1;
    }
	// Dosyadan satýr satýr oku ve listeye ekle
    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        // Satýr sonundaki yeni satýr karakterini (\n) kaldýr
        satir[strcspn(satir, "\n")] = '\0';
        searchNode(&headNode, satir,T,L);
        printList(&headNode);
    }
} 
   // 1 harici deger girilirse adresler kullanýcýdan istenir
   else {
   	 printf("String giriniz (Bitirmek icin 'exit' yazin):");
 
     while(flag==0){
	 printf("\n> ");
     scanf("%s", satir);
	   if(strcmp(satir, "exit") == 0){
	   	 	flag=1;
	  }
	else{
        searchNode(&headNode, satir,T,L);
        printList(&headNode);	
		}
	 }
   }
    printf("\nListeyi silmek istiyorsaniz  1'e basin\n");
    scanf("%d",&choice);
    if(choice==1){
       while(headNode!=NULL){
       	deleteAtEnd(&headNode);
	   }
	   printList(&headNode);	
	}
	fclose(dosya);
	return 0;
}
