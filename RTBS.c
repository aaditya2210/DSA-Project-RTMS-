#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Train structure start
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


// Function to add a new train
void addTrain()
{
    Train *newTrain = (Train *)malloc(sizeof(Train));
    if (!newTrain)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("\nEnter Train Number: ");
    scanf("%d", &newTrain->train_no);
    printf("Enter Train Name: ");
    scanf("%s", newTrain->train_name);
    printf("Enter Source Station: ");
    scanf("%s", newTrain->source);
    printf("Enter Destination Station: ");
    scanf("%s", newTrain->destination);
    printf("Enter Total Seats: ");
    scanf("%d", &newTrain->total_seats);

    newTrain->available_seats = newTrain->total_seats;
    newTrain->next = NULL;

    if (head == NULL)
    {
        head = newTrain;
    }
    else
    {
        Train *temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newTrain;
    }

    printf("\nTrain added successfully!\n");
}


// Function to display all trains
void displayTrains()
{
    if (head == NULL)
    {
        printf("\nNo trains available!\n");
        return;
    }
    printf("\n%-10s %-20s %-15s %-15s %-10s\n",
           "Train No", "Train Name", "Source", "Destination", "Available");
    printf("---------------------------------------------------------------------\n");

    Train *temp = head;
    while (temp != NULL)
    {
        printf("%-10d %-20s %-15s %-15s %-10d\n",
               temp->train_no, temp->train_name, temp->source,
               temp->destination, temp->available_seats);
        temp = temp->next;
    }
}


// Function to search for a train by number
Train* searchTrain(int train_no)
{
    Train *temp = head;
    while (temp != NULL)
    {
        if (temp->train_no == train_no)
            return temp;
        temp = temp->next;
    }
    return NULL;
}


// Preload some trains
    void initializeTrains() {
        Train *t1 = (Train*)malloc(sizeof(Train));
        t1->train_no = 20960;
        strcpy(t1->train_name, "Rajdhani Express");
        strcpy(t1->source, "Mumbai");
        strcpy(t1->destination, "Ahmedabad");
        t1->total_seats = 700;
        t1->available_seats = 120;
        t1->next = NULL;
        head = t1;
    
        Train *t2 = (Train*)malloc(sizeof(Train));
        t2->train_no = 12934;
        strcpy(t2->train_name, "Karnavati Express");
        strcpy(t2->source, "Ahmedabad");
        strcpy(t2->destination, "Mumbai");
        t2->total_seats = 720;
        t2->available_seats = 150;
        t2->next = NULL;
        t1->next = t2;
    
        Train *t3 = (Train*)malloc(sizeof(Train));
        t3->train_no = 12267;
        strcpy(t3->train_name, "Nanda Devi Express");
        strcpy(t3->source, "Dehradun");
        strcpy(t3->destination, "New Delhi");
        t3->total_seats = 680;
        t3->available_seats = 85;
        t3->next = NULL;
        t2->next = t3;
    
        Train *t4 = (Train*)malloc(sizeof(Train));
        t4->train_no = 12627;
        strcpy(t4->train_name, "Karnataka Express");
        strcpy(t4->source, "Bengaluru");
        strcpy(t4->destination, "New Delhi");
        t4->total_seats = 900;
        t4->available_seats = 230;
        t4->next = NULL;
        t3->next = t4;
    
        Train *t5 = (Train*)malloc(sizeof(Train));
        t5->train_no = 12009;
        strcpy(t5->train_name, "Shatabdi Express");
        strcpy(t5->source, "Mumbai");
        strcpy(t5->destination, "Ahmedabad");
        t5->total_seats = 800;
        t5->available_seats = 190;
        t5->next = NULL;
        t4->next = t5;
    
        Train *t6 = (Train*)malloc(sizeof(Train));
        t6->train_no = 12864;
        strcpy(t6->train_name, "Yesvantpur Express");
        strcpy(t6->source, "Howrah");
        strcpy(t6->destination, "Yesvantpur");
        t6->total_seats = 1000;
        t6->available_seats = 400;
        t6->next = NULL;
        t5->next = t6;
    
        Train *t7 = (Train*)malloc(sizeof(Train));
        t7->train_no = 12246;
        strcpy(t7->train_name, "Duronto Express");
        strcpy(t7->source, "Howrah");
        strcpy(t7->destination, "Yesvantpur");
        t7->total_seats = 950;
        t7->available_seats = 320;
        t7->next = NULL;
        t6->next = t7;
    
        Train *t8 = (Train*)malloc(sizeof(Train));
        t8->train_no = 12616;
        strcpy(t8->train_name, "Grand Trunk Express");
        strcpy(t8->source, "Chennai");
        strcpy(t8->destination, "New Delhi");
        t8->total_seats = 980;
        t8->available_seats = 275;
        t8->next = NULL;
        t7->next = t8;
    
        Train *t9 = (Train*)malloc(sizeof(Train));
        t9->train_no = 12301;
        strcpy(t9->train_name, "Rajdhani Express");
        strcpy(t9->source, "Howrah");
        strcpy(t9->destination, "New Delhi");
        t9->total_seats = 850;
        t9->available_seats = 220;
        t9->next = NULL;
        t8->next = t9;
    
        Train *t10 = (Train*)malloc(sizeof(Train));
        t10->train_no = 12622;
        strcpy(t10->train_name, "Tamil Nadu Express");
        strcpy(t10->source, "New Delhi");
        strcpy(t10->destination, "Chennai");
        t10->total_seats = 1000;
        t10->available_seats = 410;
        t10->next = NULL;
        t9->next = t10;
    
        printf("\n✅ 10 Trains initialized successfully!\n");
    }
    
// Train structure end 



// Passenger structure start
typedef struct Passenger{
    int id;
    char name[50];
    int age;
    int train_no;
    struct Passenger *next;
}Passenger;

int passengerCounter = 1;    
Passenger *bookedList  = NULL;

// Wating Queue structure start
typedef struct WaitingQueue{
    Passenger passenger;
    struct WaitingQueue *next;
}WaitingQueue;

WaitingQueue *front = NULL;
WaitingQueue *rear = NULL;


void enqueue(Passenger p) {
    WaitingQueue *newNode = (WaitingQueue*)malloc(sizeof(WaitingQueue));
    newNode->passenger = p;
    newNode->next = NULL;
    if (rear == NULL) {
        front = rear = newNode;
        return;
    }
    rear->next = newNode;
    rear = newNode;
}

Passenger dequeue() {
    Passenger empty = {0, "", 0, 0, NULL};
    if (front == NULL)
        return empty;

    WaitingQueue *temp = front;
    Passenger data = temp->passenger;
    front = front->next;
    if (front == NULL)
        rear = NULL;
    free(temp);
    return data;
}

int isQueueEmpty() {
    return front == NULL;
}

// Waiting Queue structure end


void bookTicket() {
    int train_no;
    printf("\nEnter Train Number: ");
    scanf("%d", &train_no);

    Train *train = searchTrain(train_no);
    if (train == NULL) {
        printf("❌ Train not found!\n");
        return;
    }

    Passenger *newPassenger = (Passenger*)malloc(sizeof(Passenger));
    newPassenger->id = passengerCounter++;
    newPassenger->train_no = train_no;
    printf("Enter Passenger Name: ");
    scanf("%s", newPassenger->name);
    printf("Enter Age: ");
    scanf("%d", &newPassenger->age);
    newPassenger->next = NULL;

    if (train->available_seats > 0) {
        train->available_seats--;

        if (bookedList == NULL)
            bookedList = newPassenger;
        else {
            Passenger *temp = bookedList;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newPassenger;
        }
        printf("\n✅ Ticket booked successfully! Passenger ID: %d\n", newPassenger->id);
    } else {
        enqueue(*newPassenger);
        printf("\n⚠️ No seats available! Added to waiting list.\n");
        free(newPassenger);
    }
}

void cancelTicket() {
    int id;
    printf("\nEnter Passenger ID to Cancel: ");
    scanf("%d", &id);

    Passenger *temp = bookedList, *prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("❌ Passenger not found!\n");
        return;
    }

    Train *train = searchTrain(temp->train_no);
    if (train)
        train->available_seats++;

    if (prev == NULL)
        bookedList = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("✅ Ticket cancelled successfully!\n");

    // Move waiting passenger (if any) to confirmed list
    if (!isQueueEmpty() && train != NULL) {
        Passenger nextPass = dequeue();
        Passenger *newP = (Passenger*)malloc(sizeof(Passenger));
        *newP = nextPass;
        newP->next = NULL;

        train->available_seats--;
        if (bookedList == NULL)
            bookedList = newP;
        else {
            Passenger *t = bookedList;
            while (t->next != NULL)
                t = t->next;
            t->next = newP;
        }
        printf("🎟️ Waiting passenger %s moved to confirmed list!\n", nextPass.name);
    }
}

void displayBookedPassengers() {
    if (bookedList == NULL) {
        printf("\nNo confirmed passengers yet!\n");
        return;
    }

    printf("\n%-5s %-15s %-10s %-10s\n", "ID", "Name", "Age", "Train No");
    printf("-----------------------------------------\n");

    Passenger *temp = bookedList;
    while (temp != NULL) {
        printf("%-5d %-15s %-10d %-10d\n",
               temp->id, temp->name, temp->age, temp->train_no);
        temp = temp->next;
    }
}

// Passenger structure end


// ==================== REPORT & SEARCH MODULE ====================

void displayWaitingList() {
    if (front == NULL) {
        printf("\nNo passengers in waiting list!\n");
        return;
    }

    printf("\n%-5s %-15s %-10s %-10s\n", "ID", "Name", "Age", "Train No");
    printf("-----------------------------------------\n");

    WaitingQueue *temp = front;
    while (temp != NULL) {
        printf("%-5d %-15s %-10d %-10d\n",
               temp->passenger.id, temp->passenger.name,
               temp->passenger.age, temp->passenger.train_no);
        temp = temp->next;
    }
}

void searchPassengerByTrain() {
    if (bookedList == NULL) {
        printf("\nNo confirmed bookings available!\n");
        return;
    }

    int train_no, found = 0;
    printf("\nEnter Train Number to Search Passengers: ");
    scanf("%d", &train_no);

    Passenger *temp = bookedList;
    printf("\nPassengers in Train %d:\n", train_no);
    printf("%-5s %-15s %-10s\n", "ID", "Name", "Age");
    printf("-------------------------------\n");

    while (temp != NULL) {
        if (temp->train_no == train_no) {
            printf("%-5d %-15s %-10d\n", temp->id, temp->name, temp->age);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found)
        printf("No passengers booked in this train!\n");
}

void showSystemSummary() {
    int totalTrains = 0, totalConfirmed = 0, totalWaiting = 0;

    Train *t = head;
    printf("\n==== TRAIN STATUS SUMMARY ====\n");
    printf("%-10s %-15s %-15s %-15s %-10s %-10s\n",
           "Train No", "Name", "Source", "Destination", "Booked", "Available");
    printf("---------------------------------------------------------------\n");

    while (t != NULL) {
        int booked = t->total_seats - t->available_seats;
        totalTrains++;
        totalConfirmed += booked;
        printf("%-10d %-15s %-15s %-15s %-10d %-10d\n",
               t->train_no, t->train_name, t->source, t->destination,
               booked, t->available_seats);
        t = t->next;
    }

    WaitingQueue *q = front;
    while (q != NULL) {
        totalWaiting++;
        q = q->next;
    }

    printf("\n===== SYSTEM SUMMARY =====\n");
    printf("Total Trains: %d\n", totalTrains);
    printf("Confirmed Bookings: %d\n", totalConfirmed);
    printf("Waiting List Count: %d\n", totalWaiting);
}

// Main function
int main() {
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
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addTrain(); break;
            case 2: displayTrains(); break;
            case 3: bookTicket(); break;
            case 4: cancelTicket(); break;
            case 5: displayBookedPassengers(); break;
            case 6: displayWaitingList(); break;
            case 7: searchPassengerByTrain(); break;
            case 8: showSystemSummary(); break;
            case 9: printf("\n🚉 Exiting System...\n"); exit(0);
            default: printf("❌ Invalid choice!\n");
        }
    }

    return 0;
}