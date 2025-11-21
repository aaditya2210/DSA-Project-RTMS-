#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================
   SIMPLE XOR HASH (xorHash)
   ============================ */
void xorHash(char input[], char output[])
{
    unsigned int hash = 0;
    for (int i = 0; input[i] != '\0'; i++)
    {
        hash ^= (unsigned char)input[i];
        hash = (hash << 1) | (hash >> 31);
    }
    sprintf(output, "%08x", hash);
}

/* ============================
   MERKLE TREE STRUCTURES
   ============================ */
typedef struct MerkleNode
{
    char hash[33];
    struct MerkleNode *left;
    struct MerkleNode *right;
} MerkleNode;

/* globals for merkle */
MerkleNode *leafNodes[500];
int leafCount = 0;
MerkleNode *merkleRoot = NULL;

/* helper: create node */
MerkleNode *createNode(char h[])
{
    MerkleNode *n = (MerkleNode *)malloc(sizeof(MerkleNode));
    if (!n) { printf("Memory error\n"); exit(1); }
    strncpy(n->hash, h, 32);
    n->hash[32] = '\0';
    n->left = n->right = NULL;
    return n;
}

/* combineHash: order-independent combine (sort lexicographically) */
void combineHash(char h1[], char h2[], char out[])
{
    char a[33], b[33];
    if (strcmp(h1, h2) <= 0) {
        strcpy(a, h1);
        strcpy(b, h2);
    } else {
        strcpy(a, h2);
        strcpy(b, h1);
    }
    char combined[100];
    sprintf(combined, "%s%s", a, b);
    xorHash(combined, out);
}

/* create passenger leaf hash */
void createPassengerHash(int id, char name[], int age, int trainNo, char out[])
{
    char data[200];
    sprintf(data, "%d|%s|%d|%d", id, name, age, trainNo);
    xorHash(data, out);
}

/* add leaf node (used when booking) */
void addLeafNode(char leafHash[])
{
    if (leafCount >= 500) return;
    leafNodes[leafCount++] = createNode(leafHash);
}

/* Build Merkle Tree (bottom-up). Handles odd nodes by duplicating last node */
MerkleNode *buildMerkleTree()
{
    if (leafCount == 0) return NULL;

    MerkleNode *level[500];
    int count = leafCount;
    for (int i = 0; i < count; i++) level[i] = leafNodes[i];

    while (count > 1) {
        MerkleNode *nextLevel[500];
        int newCount = 0;

        for (int i = 0; i < count; i += 2) {
            if (i + 1 == count) {
                // odd - duplicate last
                char dupHash[33];
                combineHash(level[i]->hash, level[i]->hash, dupHash);
                MerkleNode *parent = createNode(dupHash);
                parent->left = level[i];
                parent->right = level[i];
                nextLevel[newCount++] = parent;
            } else {
                char newHash[33];
                combineHash(level[i]->hash, level[i+1]->hash, newHash);
                MerkleNode *parent = createNode(newHash);
                parent->left = level[i];
                parent->right = level[i+1];
                nextLevel[newCount++] = parent;
            }
        }

        // move nextLevel to level
        for (int j = 0; j < newCount; j++) level[j] = nextLevel[j];
        count = newCount;
    }

    return level[0]; // root
}

/* getProof: collects sibling hashes bottom->top into proof[][33] */
/* proofLen is incremented for each sibling appended */
int getProof(MerkleNode *node, char target[], char proof[][33], int *proofLen)
{
    if (node == NULL) return 0;

    // leaf
    if (node->left == NULL && node->right == NULL) {
        return strcmp(node->hash, target) == 0;
    }

    // search left subtree
    if (node->left && getProof(node->left, target, proof, proofLen)) {
        if (node->right) {
            strcpy(proof[*proofLen], node->right->hash);
        } else {
            // odd -> duplicate left
            strcpy(proof[*proofLen], node->left->hash);
        }
        (*proofLen)++;
        return 1;
    }

    // search right subtree
    if (node->right && getProof(node->right, target, proof, proofLen)) {
        if (node->left) {
            strcpy(proof[*proofLen], node->left->hash);
        } else {
            // odd -> duplicate right
            strcpy(proof[*proofLen], node->right->hash);
        }
        (*proofLen)++;
        return 1;
    }

    return 0;
}

/* verifyProof: apply proof bottom->top using combineHash */
int verifyProof(char leaf[], char proof[][33], int proofLen, char root[])
{
    char current[33];
    strcpy(current, leaf);

    for (int i = 0; i < proofLen; i++) {
        char newHash[33];
        combineHash(current, proof[i], newHash);
        strcpy(current, newHash);
    }

    return strcmp(current, root) == 0;
}

/* ============================
   TRAIN + PASSENGER + QUEUE
   ============================ */

typedef struct Train
{
    int train_no;
    char train_name[50];
    char source[30];
    char destination[30];
    int total_seats;
    int available_seats;
    struct Train *next;
} Train;

Train *head = NULL;

typedef struct Passenger
{
    int id;
    char name[50];
    int age;
    int train_no;
    struct Passenger *next;
} Passenger;

Passenger *bookedList = NULL;
int passengerCounter = 1;

typedef struct WaitingQueue
{
    Passenger passenger;
    struct WaitingQueue *next;
} WaitingQueue;

WaitingQueue *front = NULL;
WaitingQueue *rear = NULL;

/* queue functions */
void enqueue(Passenger p)
{
    WaitingQueue *n = (WaitingQueue *)malloc(sizeof(WaitingQueue));
    n->passenger = p;
    n->next = NULL;
    if (rear == NULL) front = rear = n;
    else { rear->next = n; rear = n; }
}

Passenger dequeue()
{
    Passenger empty = {0, "", 0, 0, NULL};
    if (front == NULL) return empty;
    WaitingQueue *t = front;
    Passenger p = t->passenger;
    front = front->next;
    if (front == NULL) rear = NULL;
    free(t);
    return p;
}

int isQueueEmpty() { return front == NULL; }

/* train helpers */
Train *searchTrain(int train_no)
{
    Train *t = head;
    while (t) {
        if (t->train_no == train_no) return t;
        t = t->next;
    }
    return NULL;
}

void addTrain()
{
    Train *newTrain = (Train *)malloc(sizeof(Train));
    if (!newTrain) { printf("Memory fail\n"); return; }
    printf("\nEnter Train Number: "); scanf("%d", &newTrain->train_no);
    printf("Enter Train Name: "); scanf("%s", newTrain->train_name);
    printf("Enter Source Station: "); scanf("%s", newTrain->source);
    printf("Enter Destination Station: "); scanf("%s", newTrain->destination);
    printf("Enter Total Seats: "); scanf("%d", &newTrain->total_seats);
    newTrain->available_seats = newTrain->total_seats;
    newTrain->next = NULL;
    if (head == NULL) head = newTrain;
    else {
        Train *t = head;
        while (t->next) t = t->next;
        t->next = newTrain;
    }
    printf("\nTrain added successfully!\n");
}

void displayTrains()
{
    if (!head) { printf("\nNo trains available!\n"); return; }
    printf("\n%-10s %-20s %-15s %-15s %-10s\n", "Train No", "Train Name", "Source", "Destination", "Available");
    printf("---------------------------------------------------------------------\n");
    Train *t = head;
    while (t) {
        printf("%-10d %-20s %-15s %-15s %-10d\n", t->train_no, t->train_name, t->source, t->destination, t->available_seats);
        t = t->next;
    }
}

/* preload trains */
void initializeTrains()
{
    Train *t1 = (Train *)malloc(sizeof(Train));
    t1->train_no = 20960; strcpy(t1->train_name, "Rajdhani Express"); strcpy(t1->source, "Mumbai"); strcpy(t1->destination, "Ahmedabad"); t1->total_seats = 700; t1->available_seats = 120; t1->next = NULL; head = t1;
    Train *t2 = (Train *)malloc(sizeof(Train));
    t2->train_no = 12934; strcpy(t2->train_name, "Karnavati Express"); strcpy(t2->source, "Ahmedabad"); strcpy(t2->destination, "Mumbai"); t2->total_seats = 720; t2->available_seats = 1; t2->next = NULL; t1->next = t2;
    Train *t3 = (Train *)malloc(sizeof(Train));
    t3->train_no = 12267; strcpy(t3->train_name, "Nanda Devi Express"); strcpy(t3->source, "Dehradun"); strcpy(t3->destination, "New Delhi"); t3->total_seats = 680; t3->available_seats = 85; t3->next = NULL; t2->next = t3;
    Train *t4 = (Train *)malloc(sizeof(Train));
    t4->train_no = 12627; strcpy(t4->train_name, "Karnataka Express"); strcpy(t4->source, "Bengaluru"); strcpy(t4->destination, "New Delhi"); t4->total_seats = 900; t4->available_seats = 230; t4->next = NULL; t3->next = t4;
    Train *t5 = (Train *)malloc(sizeof(Train));
    t5->train_no = 12009; strcpy(t5->train_name, "Shatabdi Express"); strcpy(t5->source, "Mumbai"); strcpy(t5->destination, "Ahmedabad"); t5->total_seats = 800; t5->available_seats = 190; t5->next = NULL; t4->next = t5;
    Train *t6 = (Train *)malloc(sizeof(Train));
    t6->train_no = 12864; strcpy(t6->train_name, "Yesvantpur Express"); strcpy(t6->source, "Howrah"); strcpy(t6->destination, "Yesvantpur"); t6->total_seats = 1000; t6->available_seats = 400; t6->next = NULL; t5->next = t6;
    Train *t7 = (Train *)malloc(sizeof(Train));
    t7->train_no = 12246; strcpy(t7->train_name, "Duronto Express"); strcpy(t7->source, "Howrah"); strcpy(t7->destination, "Yesvantpur"); t7->total_seats = 950; t7->available_seats = 320; t7->next = NULL; t6->next = t7;
    Train *t8 = (Train *)malloc(sizeof(Train));
    t8->train_no = 12616; strcpy(t8->train_name, "Grand Trunk Express"); strcpy(t8->source, "Chennai"); strcpy(t8->destination, "New Delhi"); t8->total_seats = 980; t8->available_seats = 275; t8->next = NULL; t7->next = t8;
    Train *t9 = (Train *)malloc(sizeof(Train));
    t9->train_no = 12301; strcpy(t9->train_name, "Rajdhani Express"); strcpy(t9->source, "Howrah"); strcpy(t9->destination, "New Delhi"); t9->total_seats = 850; t9->available_seats = 220; t9->next = NULL; t8->next = t9;
    Train *t10 = (Train *)malloc(sizeof(Train));
    t10->train_no = 12622; strcpy(t10->train_name, "Tamil Nadu Express"); strcpy(t10->source, "New Delhi"); strcpy(t10->destination, "Chennai"); t10->total_seats = 1000; t10->available_seats = 410; t10->next = NULL; t9->next = t10;

    printf("\n10 Trains initialized successfully!\n");
}

/* ============================
   BOOK / CANCEL / PRINT / VERIFY
   ============================ */

void rebuildLeavesFromBooked()
{
    leafCount = 0;
    Passenger *p = bookedList;
    while (p) {
        char h[33];
        createPassengerHash(p->id, p->name, p->age, p->train_no, h);
        addLeafNode(h);
        p = p->next;
    }
    merkleRoot = buildMerkleTree();
}

void bookTicket()
{
    int train_no;
    printf("\nEnter Train Number: ");
    scanf("%d", &train_no);

    Train *tr = searchTrain(train_no);
    if (!tr) { printf("Train not found!\n"); return; }

    Passenger *p = (Passenger *)malloc(sizeof(Passenger));
    p->id = passengerCounter++;
    p->train_no = train_no;
    printf("Enter Passenger Name: "); scanf("%s", p->name);
    printf("Enter Age: "); scanf("%d", &p->age);
    p->next = NULL;

    if (tr->available_seats > 0) {
        tr->available_seats--;
        if (bookedList == NULL) bookedList = p;
        else {
            Passenger *t = bookedList;
            while (t->next) t = t->next;
            t->next = p;
        }

        // add leaf & rebuild small (simple)
        char h[33];
        createPassengerHash(p->id, p->name, p->age, p->train_no, h);
        addLeafNode(h);
        merkleRoot = buildMerkleTree();

        printf("\nTicket booked successfully! Passenger ID: %d\n", p->id);
    } else {
        enqueue(*p);
        free(p);
        printf("\nNo seats available! Added to waiting list.\n");
    }
}

void cancelTicket()
{
    int id;
    printf("\nEnter Passenger ID to Cancel: ");
    scanf("%d", &id);

    Passenger *temp = bookedList, *prev = NULL;
    while (temp && temp->id != id) { prev = temp; temp = temp->next; }

    if (!temp) { printf("Passenger not found!\n"); return; }

    Train *tr = searchTrain(temp->train_no);
    if (tr) tr->available_seats++;

    if (prev == NULL) bookedList = temp->next;
    else prev->next = temp->next;

    free(temp);
    printf("Ticket cancelled successfully!\n");

    // move waiting passenger if any
    if (!isQueueEmpty() && tr != NULL) {
        Passenger w = dequeue();
        Passenger *np = (Passenger *)malloc(sizeof(Passenger));
        *np = w; np->next = NULL;
        tr->available_seats--;
        if (bookedList == NULL) bookedList = np;
        else {
            Passenger *t = bookedList;
            while (t->next) t = t->next;
            t->next = np;
        }
        printf("Waiting passenger %s moved to confirmed list!\n", w.name);
    }

    // rebuild leaves and root
    rebuildLeavesFromBooked();
}

void displayBookedPassengers()
{
    if (!bookedList) { printf("\nNo confirmed passengers yet!\n"); return; }
    printf("\n%-5s %-15s %-10s %-10s\n", "ID", "Name", "Age", "Train No");
    printf("-----------------------------------------\n");
    Passenger *p = bookedList;
    while (p) {
        printf("%-5d %-15s %-10d %-10d\n", p->id, p->name, p->age, p->train_no);
        p = p->next;
    }
}

void printTicket()
{
    int id;
    printf("\nEnter Passenger ID to Print Ticket: ");
    scanf("%d", &id);

    Passenger *p = bookedList;
    while (p && p->id != id) p = p->next;
    if (!p) { printf("\nPassenger not found or not confirmed!\n"); return; }

    Train *tr = searchTrain(p->train_no);
    if (!tr) { printf("\nTrain details not found!\n"); return; }

    char leafHash[33];
    createPassengerHash(p->id, p->name, p->age, p->train_no, leafHash);

    char proof[50][33];
    int proofLen = 0;
    if (merkleRoot) getProof(merkleRoot, leafHash, proof, &proofLen);

    printf("\n=============== SECURITY DATA ===============\n");
    printf("Leaf Hash  : %s\n", leafHash);
    if (merkleRoot) printf("Merkle Root: %s\n", merkleRoot->hash);
    else printf("Merkle Root: (none)\n");

    for (int i = 0; i < proofLen; i++)
        printf("Proof[%d]   : %s\n", i, proof[i]);

    printf("=============================================\n");

    printf("\n================= TRAIN TICKET =================\n");
    printf("           Indian Railways - E-Ticket\n");
    printf("------------------------------------------------\n");
    printf(" Passenger ID     : %d\n", p->id);
    printf(" Passenger Name   : %s\n", p->name);
    printf(" Age              : %d\n", p->age);
    printf(" Train No         : %d\n", tr->train_no);
    printf(" Train Name       : %s\n", tr->train_name);
    printf(" Source Station   : %s\n", tr->source);
    printf(" Destination      : %s\n", tr->destination);
    printf(" Seat Status      : CONFIRMED\n");
    printf("------------------------------------------------\n");
}

void displayWaitingList()
{
    if (front == NULL) { printf("\nNo passengers in waiting list!\n"); return; }
    printf("\n%-5s %-15s %-10s %-10s\n", "ID", "Name", "Age", "Train No");
    printf("-----------------------------------------\n");
    WaitingQueue *t = front;
    while (t) {
        printf("%-5d %-15s %-10d %-10d\n", t->passenger.id, t->passenger.name, t->passenger.age, t->passenger.train_no);
        t = t->next;
    }
}

void searchPassengerByTrain()
{
    if (!bookedList) { printf("\nNo confirmed bookings available!\n"); return; }
    int train_no, found = 0;
    printf("\nEnter Train Number to Search Passengers: ");
    scanf("%d", &train_no);
    Passenger *p = bookedList;
    printf("\nPassengers in Train %d:\n", train_no);
    printf("%-5s %-15s %-10s\n", "ID", "Name", "Age");
    printf("-------------------------------\n");
    while (p) {
        if (p->train_no == train_no) {
            printf("%-5d %-15s %-10d\n", p->id, p->name, p->age);
            found = 1;
        }
        p = p->next;
    }
    if (!found) printf("No passengers booked in this train!\n");
}

void showSystemSummary()
{
    int totalTrains = 0, totalConfirmed = 0, totalWaiting = 0;
    Train *t = head;
    printf("\n==== TRAIN STATUS SUMMARY ====\n");
    printf("%-10s %-15s %-15s %-15s %-10s %-10s\n", "Train No", "Name", "Source", "Destination", "Booked", "Available");
    printf("---------------------------------------------------------------\n");
    while (t) {
        int booked = t->total_seats - t->available_seats;
        totalTrains++;
        totalConfirmed += booked;
        printf("%-10d %-15s %-15s %-15s %-10d %-10d\n", t->train_no, t->train_name, t->source, t->destination, booked, t->available_seats);
        t = t->next;
    }
    WaitingQueue *q = front;
    while (q) { totalWaiting++; q = q->next; }
    printf("\n===== SYSTEM SUMMARY =====\n");
    printf("Total Trains: %d\n", totalTrains);
    printf("Confirmed Bookings: %d\n", totalConfirmed);
    printf("Waiting List Count: %d\n", totalWaiting);
}

/* ============================
   VERIFY FUNCTION
   ============================ */
void verifyTicket()
{
    if (bookedList == NULL || merkleRoot == NULL) {
        printf("\nNo tickets or Merkle Tree not built!\n");
        return;
    }

    int id;
    printf("\nEnter Passenger ID to Verify: ");
    scanf("%d", &id);

    Passenger *p = bookedList;
    while (p && p->id != id) p = p->next;
    if (!p) { printf("\nPassenger not found!\n"); return; }

    char leafHash[33];
    createPassengerHash(p->id, p->name, p->age, p->train_no, leafHash);

    char proof[50][33];
    int proofLen = 0;
    getProof(merkleRoot, leafHash, proof, &proofLen);

    int ok = verifyProof(leafHash, proof, proofLen, merkleRoot->hash);

    printf("\n=========== TICKET VERIFICATION ===========\n");
    printf(" Passenger ID : %d\n", p->id);
    printf(" Leaf Hash    : %s\n", leafHash);
    printf(" Root Hash    : %s\n", merkleRoot->hash);

    if (ok) printf("\n STATUS: ✅ VALID TICKET (Authentic)\n");
    else printf("\n STATUS: ❌ INVALID / TAMPERED TICKET\n");

    printf("===========================================\n");
}

/* ============================
   MAIN
   ============================ */

int main()
{
    int choice;
    initializeTrains();

    while (1) {
        printf("\n==== RAILWAY TICKET BOOKING SYSTEM ====\n");
        printf("1. Add Train\n");
        printf("2. Display All Trains\n");
        printf("3. Book Ticket\n");
        printf("4. Cancel Ticket\n");
        printf("5. Show Booked Passengers\n");
        printf("6. Show Waiting List\n");
        printf("7. Search Passenger by Train\n");
        printf("8. System Summary Report\n");
        printf("9. Print Ticket by Passenger ID\n");
        printf("10. Verify Ticket Authenticity\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) { fflush(stdin); continue; }

        switch (choice) {
            case 1: addTrain(); break;
            case 2: displayTrains(); break;
            case 3: bookTicket(); break;
            case 4: cancelTicket(); break;
            case 5: displayBookedPassengers(); break;
            case 6: displayWaitingList(); break;
            case 7: searchPassengerByTrain(); break;
            case 8: showSystemSummary(); break;
            case 9: printTicket(); break;
            case 10: verifyTicket(); break;
            case 11: printf("\nExiting System...\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}
