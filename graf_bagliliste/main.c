#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 100 // Maksimum yýðýn boyutu

int dugum_sayisi = 0;

struct node
{
    char sehirAdi[15];
    struct node *next; // ilk oluþturulan baðlý liste için

    int counter; // her düðümün komþu sayýsý
    struct node *komsular[100]; // her bir düðümün komþularýný tutmak için
    int komsuAgirlik[100]; // her bir komþu düðümün aðýrlýklarýný tutmak için

    int state; // ziyaret durumu
    int distance; // Dijkstra ve Bellman-Ford algoritmalarý için mesafe deðeri
};

struct node *start = NULL;
struct node *last = NULL;
struct node *temp;
struct node *aranan;

void sehirEkle(char sehir[15])
{
    dugum_sayisi++;
    struct node *dugum = (struct node *)malloc(sizeof(struct node));
    strcpy(dugum->sehirAdi, sehir);
    dugum->next = NULL;
    dugum->state = 0; // ziyaret edilmedi
    dugum->counter = 0;
    dugum->distance = INT_MAX; // Sonsuz mesafe olarak baþlat
    if (start == NULL)
    {
        start = dugum;
        last = dugum;
    }
    else
    {
        last->next = dugum;
        last = dugum;
    }
}

void sehirYazdir()
{
    temp = start;
    while (temp->next != NULL)
    {
        printf("%s ", temp->sehirAdi);
        temp = temp->next;
    }
    printf("%s ", temp->sehirAdi);
}

void dugumVarMi(char dugum[15])
{
    temp = start;
    while (strcmp(temp->sehirAdi, dugum) != 0)   //0'a eþit olsa eþleþmiþ demektir
    {
        temp = temp->next;
        if (temp->next == NULL)   //aranan düðüm daha önce baðlý listeye eklenmemiþse
        {
            sehirEkle(dugum);
            temp = last;
        }
    }
    aranan = temp;
}

void komsuEkle(char src[15], char dest[15], int agirlik)
{
    struct node *source;
    dugumVarMi(src);
    source = aranan;
    struct node *destination;
    dugumVarMi(dest);
    destination = aranan;
    source->komsular[source->counter] = destination;
    source->komsuAgirlik[source->counter] = agirlik;
    source->counter++;
    destination->komsular[destination->counter] = source;     ///yonsuz graflar icin
    destination->komsuAgirlik[destination->counter] = agirlik;
    destination->counter++;
}

void komsuYazdir()
{
    temp = start;
    while (temp->next != NULL)
    {
        printf("\n%s : ", temp->sehirAdi);
        for (int i = 0; i < temp->counter; i++)
        {
            printf("%s ", temp->komsular[i]->sehirAdi);
        }
        temp = temp->next;
    }
}

void bfs(char sehir[15])
{
    struct node *queue[100]; // BFS sýrasýnda ziyaret edilen düðümleri tutmak için kuyruk
    int front = 0, rear = 0; // Kuyrukta ön ve arka iþaretçileri

    // Baþlangýç düðümünü iþaretle ve kuyruða ekle
    dugumVarMi(sehir);
    aranan->state = 1; // Ziyaret edildi olarak iþaretle
    queue[rear++] = aranan;

    while (front != rear)   // Kuyruk boþ olana kadar
    {
        struct node *current = queue[front++]; // Kuyruktan bir düðüm al
        printf("%s ", current->sehirAdi); // Düðümü yazdýr

        // Düðümün komþularýný kontrol et
        for (int i = 0; i < current->counter; i++)
        {
            struct node *neighbor = current->komsular[i];
            if (neighbor->state == 0)   // Eðer komþu henüz ziyaret edilmediyse
            {
                neighbor->state = 1; // Ziyaret edildi olarak iþaretle
                queue[rear++] = neighbor; // Kuyruða ekle
            }
        }
    }

    // BFS sonrasý tüm düðümleri baþlangýç durumuna geri döndür
    temp = start;
    while (temp != NULL)
    {
        temp->state = 0;
        temp = temp->next;
    }
}

// Yýðýn için global deðiþkenler
struct node *stack[MAX];
int top = -1;
void push(struct node *item){
    if (top == MAX - 1){
        printf("Yigin dolu!\n");
        return;
    }
    stack[++top] = item;
}
struct node *pop(){
    if (top == -1){
        printf("Yigin bos!\n");
        exit(1);
    }
    return stack[top--];
}
void dfs(char sehir[15]){
    // DFS için baþlangýç düðümünü iþaretle ve yýðýna ekle
    dugumVarMi(sehir);
    aranan->state = 1; // Ziyaret edildi olarak iþaretle
    push(aranan);
    while (top != -1){  // Yýðýn boþ olana kadar
        struct node *current = pop(); // Yýðýndan bir düðüm al
        printf("%s ", current->sehirAdi); // Düðümü yazdýr

        // Düðümün komþularýný kontrol et
        for (int i = 0; i < current->counter; i++)
        {
            struct node *neighbor = current->komsular[i];
            if (neighbor->state == 0)   // Eðer komþu henüz ziyaret edilmediyse
            {
                neighbor->state = 1; // Ziyaret edildi olarak iþaretle
                push(neighbor); // Yýðýna ekle
            }
        }
    }
    // DFS sonrasý tüm düðümleri baþlangýç durumuna geri döndür
    temp = start;
    while (temp != NULL){
        temp->state = 0;
        temp = temp->next;
    }
}

void dijkstra(char baslangic[15])
{
    // Baþlangýç düðümünü bul ve mesafe deðerini 0 olarak ayarla
    dugumVarMi(baslangic);
    aranan->distance = 0;
    // Ziyaret edilmemiþ düðümleri tutmak için kuyruk
    struct node *queue[100];
    int front = 0, rear = 0;
    // Baþlangýç düðümünü kuyruða ekle
    queue[rear++] = aranan;
    while (front != rear)   // Kuyruk boþ olana kadar
    {
        struct node *current = queue[front++]; // Kuyruktan bir düðüm al
        // Düðümün komþularýný kontrol et
        for (int i = 0; i < current->counter; i++)
        {
            struct node *neighbor = current->komsular[i];
            int weight = current->komsuAgirlik[i]; // Komþu düðümün aðýrlýðý
            // Eðer yeni yol daha kýsa ise mesafeyi güncelle
            if (current->distance + weight < neighbor->distance)
            {
                neighbor->distance = current->distance + weight;
                // Güncellenmiþ mesafeyi kuyruða ekle
                queue[rear++] = neighbor;
            }
        }
    }
    // Dijkstra sonrasý tüm düðümleri baþlangýç mesafesine geri döndür
    temp = start;
    while (temp->next != NULL)
    {
        if (temp->distance == INT_MAX)
            printf("Dugum %s ulasilamaz.\n", temp->sehirAdi);
        else
            printf("%s 'den %s 'ye min mesafe: %d\n", baslangic, temp->sehirAdi, temp->distance);
        temp->distance = INT_MAX;
        temp = temp->next;
    }
}

void bellmanFord(char baslangic[15])
{
    // Baþlangýç düðümünü bul
    dugumVarMi(baslangic);
    aranan->distance = 0;
    // Tüm düðümleri ve kenarlarý taramak için döngü
    for (int i = 0; i < dugum_sayisi - 1; i++)
    {
        temp = start;
        // Tüm kenarlarý tara
        while (temp != NULL)
        {
            for (int j = 0; j < temp->counter; j++)
            {
                struct node *neighbor = temp->komsular[j];
                int weight = temp->komsuAgirlik[j];
                // Eðer yeni yol daha kýsa ise mesafeyi güncelle
                if (temp->distance != INT_MAX && temp->distance + weight < neighbor->distance)
                    neighbor->distance = temp->distance + weight;
            }
            temp = temp->next;
        }
    }
    // Negatif döngü kontrolü
    temp = start;
    while (temp != NULL)
    {
        for (int i = 0; i < temp->counter; i++)
        {
            struct node *neighbor = temp->komsular[i];
            int weight = temp->komsuAgirlik[i];
            if (temp->distance != INT_MAX && temp->distance + weight < neighbor->distance)
            {
                printf("Negatif agirlikli dongu bulundu.\n");
                return;
            }
        }
        temp = temp->next;
    }
    // Bellman-Ford sonrasý tüm düðümleri baþlangýç mesafesine geri döndür
    temp = start;
    while (temp->next != NULL)
    {
        if (temp->distance == INT_MAX)
            printf("Dugum %s ulasilamaz.\n", temp->sehirAdi);
        else
            printf("En kisa mesafe %s dugumden %s dugumune: %d\n", baslangic, temp->sehirAdi, temp->distance);
        temp->distance = INT_MAX;
        temp = temp->next;
    }
}


int main()
{
    char sehir[15];
    int secim, komsuSayisi, agirlik;
    char src[15];
    char dest[15];

    while (1 == 1)
    {
        printf("\n1- Sehir Ekle ");
        printf("\n2- Sehir Yazdir");
        printf("\n3- Komsu Ekle");
        printf("\n4- Komsulari Yazdir");
        printf("\n5- BFS");
        printf("\n6- DFS");
        printf("\n7- Dijkstra");
        printf("\n8- Bellman Ford");
        printf("\nSeçiminizi giriniz: ");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
            printf("Eklemek istediginiz sehir:");
            scanf("%s", &sehir);
            sehirEkle(sehir);
            break;
        case 2:
            sehirYazdir();
            break;
        case 3:
            printf("Kaynak Dugum:");
            scanf("%s", &src);
            printf("Kac komsu girmek istiyorsunuz?");
            scanf("%d", &komsuSayisi);
            for (int i = 0; i < komsuSayisi; i++)
            {
                printf("%d.inci Komsu Dugum:", i + 1);
                scanf("%s", &dest);
                printf("%d.inci Komsu Agirligi:", i + 1);
                scanf("%d", &agirlik);
                komsuEkle(src, dest, agirlik);
            }
            break;
        case 4:
            komsuYazdir();
            break;
        case 5:
            printf("BFS baslanacak dugum:");
            scanf("%s", &sehir);
            bfs(sehir);
            break;
        case 6:
            printf("DFS baslanacak dugum:");
            scanf("%s", &sehir);
            dfs(sehir);
            break;
        case 7:
            printf("Djikstra baslanacak dugum:");
            scanf("%s", &sehir);
            dijkstra(sehir);
            break;
        case 8:
            printf("Bellman Ford baslanacak dugum:");
            scanf("%s", &sehir);
            bellmanFord(sehir);
            break;
        }
    }
    return 0;
}
