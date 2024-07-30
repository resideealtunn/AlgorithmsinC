#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX 100 // Maksimum y���n boyutu

int dugum_sayisi = 0;

struct node
{
    char sehirAdi[15];
    struct node *next; // ilk olu�turulan ba�l� liste i�in

    int counter; // her d���m�n kom�u say�s�
    struct node *komsular[100]; // her bir d���m�n kom�ular�n� tutmak i�in
    int komsuAgirlik[100]; // her bir kom�u d���m�n a��rl�klar�n� tutmak i�in

    int state; // ziyaret durumu
    int distance; // Dijkstra ve Bellman-Ford algoritmalar� i�in mesafe de�eri
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
    dugum->distance = INT_MAX; // Sonsuz mesafe olarak ba�lat
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
    while (strcmp(temp->sehirAdi, dugum) != 0)   //0'a e�it olsa e�le�mi� demektir
    {
        temp = temp->next;
        if (temp->next == NULL)   //aranan d���m daha �nce ba�l� listeye eklenmemi�se
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
    struct node *queue[100]; // BFS s�ras�nda ziyaret edilen d���mleri tutmak i�in kuyruk
    int front = 0, rear = 0; // Kuyrukta �n ve arka i�aret�ileri

    // Ba�lang�� d���m�n� i�aretle ve kuyru�a ekle
    dugumVarMi(sehir);
    aranan->state = 1; // Ziyaret edildi olarak i�aretle
    queue[rear++] = aranan;

    while (front != rear)   // Kuyruk bo� olana kadar
    {
        struct node *current = queue[front++]; // Kuyruktan bir d���m al
        printf("%s ", current->sehirAdi); // D���m� yazd�r

        // D���m�n kom�ular�n� kontrol et
        for (int i = 0; i < current->counter; i++)
        {
            struct node *neighbor = current->komsular[i];
            if (neighbor->state == 0)   // E�er kom�u hen�z ziyaret edilmediyse
            {
                neighbor->state = 1; // Ziyaret edildi olarak i�aretle
                queue[rear++] = neighbor; // Kuyru�a ekle
            }
        }
    }

    // BFS sonras� t�m d���mleri ba�lang�� durumuna geri d�nd�r
    temp = start;
    while (temp != NULL)
    {
        temp->state = 0;
        temp = temp->next;
    }
}

// Y���n i�in global de�i�kenler
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
    // DFS i�in ba�lang�� d���m�n� i�aretle ve y���na ekle
    dugumVarMi(sehir);
    aranan->state = 1; // Ziyaret edildi olarak i�aretle
    push(aranan);
    while (top != -1){  // Y���n bo� olana kadar
        struct node *current = pop(); // Y���ndan bir d���m al
        printf("%s ", current->sehirAdi); // D���m� yazd�r

        // D���m�n kom�ular�n� kontrol et
        for (int i = 0; i < current->counter; i++)
        {
            struct node *neighbor = current->komsular[i];
            if (neighbor->state == 0)   // E�er kom�u hen�z ziyaret edilmediyse
            {
                neighbor->state = 1; // Ziyaret edildi olarak i�aretle
                push(neighbor); // Y���na ekle
            }
        }
    }
    // DFS sonras� t�m d���mleri ba�lang�� durumuna geri d�nd�r
    temp = start;
    while (temp != NULL){
        temp->state = 0;
        temp = temp->next;
    }
}

void dijkstra(char baslangic[15])
{
    // Ba�lang�� d���m�n� bul ve mesafe de�erini 0 olarak ayarla
    dugumVarMi(baslangic);
    aranan->distance = 0;
    // Ziyaret edilmemi� d���mleri tutmak i�in kuyruk
    struct node *queue[100];
    int front = 0, rear = 0;
    // Ba�lang�� d���m�n� kuyru�a ekle
    queue[rear++] = aranan;
    while (front != rear)   // Kuyruk bo� olana kadar
    {
        struct node *current = queue[front++]; // Kuyruktan bir d���m al
        // D���m�n kom�ular�n� kontrol et
        for (int i = 0; i < current->counter; i++)
        {
            struct node *neighbor = current->komsular[i];
            int weight = current->komsuAgirlik[i]; // Kom�u d���m�n a��rl���
            // E�er yeni yol daha k�sa ise mesafeyi g�ncelle
            if (current->distance + weight < neighbor->distance)
            {
                neighbor->distance = current->distance + weight;
                // G�ncellenmi� mesafeyi kuyru�a ekle
                queue[rear++] = neighbor;
            }
        }
    }
    // Dijkstra sonras� t�m d���mleri ba�lang�� mesafesine geri d�nd�r
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
    // Ba�lang�� d���m�n� bul
    dugumVarMi(baslangic);
    aranan->distance = 0;
    // T�m d���mleri ve kenarlar� taramak i�in d�ng�
    for (int i = 0; i < dugum_sayisi - 1; i++)
    {
        temp = start;
        // T�m kenarlar� tara
        while (temp != NULL)
        {
            for (int j = 0; j < temp->counter; j++)
            {
                struct node *neighbor = temp->komsular[j];
                int weight = temp->komsuAgirlik[j];
                // E�er yeni yol daha k�sa ise mesafeyi g�ncelle
                if (temp->distance != INT_MAX && temp->distance + weight < neighbor->distance)
                    neighbor->distance = temp->distance + weight;
            }
            temp = temp->next;
        }
    }
    // Negatif d�ng� kontrol�
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
    // Bellman-Ford sonras� t�m d���mleri ba�lang�� mesafesine geri d�nd�r
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
        printf("\nSe�iminizi giriniz: ");
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
