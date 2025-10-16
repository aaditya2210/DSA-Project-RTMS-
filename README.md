# 🚂 Railway Ticket Booking System

A comprehensive console-based railway ticket booking system implemented in C, featuring train management, ticket booking, waiting list functionality, and passenger tracking.

## ✨ Features

### Train Management
- ➕ Add new trains with complete details
- 📋 Display all available trains
- 🔍 Search trains by train number
- 📊 Pre-loaded sample trains for testing

### Ticket Booking
- 🎫 Book tickets for available trains
- ⏳ Automatic waiting list when seats are full
- 🆔 Unique passenger ID generation
- ✅ Real-time seat availability updates

### Ticket Cancellation
- ❌ Cancel booked tickets by passenger ID
- 🔄 Automatic seat release
- 🎟️ Auto-promotion of waiting list passengers to confirmed status

### Reports & Analytics
- 👥 View all confirmed passengers
- ⏰ Display waiting list
- 🔎 Search passengers by train number
- 📈 System summary with booking statistics


## 🧩 Data Structures Used

| Data Structure | Usage |
|----------------|--------|
| **Linked List** | To store train and passenger data dynamically |
| **Queue** | To manage waiting list for ticket booking |
| **Struct** | To define trains and passengers |
| **Pointers** | For dynamic memory allocation & node linking |

---

## ⚙️ Modules

### **Module 1: Train Management**
- Add new trains  
- View all trains  
- Search trains by number  
- Data Structure: **Singly Linked List**

### **Module 2: Passenger & Ticket Booking**
- Book tickets for available seats  
- Add passengers to waiting list if full  
- Manage confirmed and waiting lists  
- Data Structure: **Queue + Linked List**

### **Module 3: Ticket Cancellation & Display**
- Cancel booked tickets  
- Move waiting list passengers into confirmed list  
- Display current booking and waiting list status  
- Data Structure: **Queue**

---

## 🚀 Getting Started

### Prerequisites
- GCC compiler or any C compiler
- Terminal/Command prompt

### Compilation

```bash
gcc railway_booking.c -o railway_booking
```

### Running the Program

```bash
./railway_booking
```

## 📖 Usage

Upon running the program, you'll see a menu with the following options:

```
==== RAILWAY TICKET BOOKING SYSTEM ====
1. Add Train
2. Display All Trains
3. Book Ticket
4. Cancel Ticket
5. Show Booked Passengers
6. Show Waiting List
7. Search Passenger by Train
8. System Summary Report
9. Exit
```

### Sample Pre-loaded Trains

| Train No | Train Name           | Source     | Destination | Total Seats | Available |
|----------|---------------------|------------|-------------|-------------|-----------|
| 20960    | Rajdhani Express    | Mumbai     | Ahmedabad   | 700         | 120       |
| 12934    | Karnavati Express   | Ahmedabad  | Mumbai      | 720         | 150       |
| 12267    | Nanda Devi Express  | Dehradun   | New Delhi   | 680         | 85        |
| 12627    | Karnataka Express   | Bengaluru  | New Delhi   | 900         | 230       |
| 12009    | Shatabdi Express    | Mumbai     | Ahmedabad   | 800         | 190       |
| 12864    | Yesvantpur Express  | Howrah     | Yesvantpur  | 1000        | 400       |
| 12246    | Duronto Express     | Howrah     | Yesvantpur  | 950         | 320       |
| 12616    | Grand Trunk Express | Chennai    | New Delhi   | 980         | 275       |
| 12301    | Rajdhani Express    | Howrah     | New Delhi   | 850         | 220       |
| 12622    | Tamil Nadu Express  | New Delhi  | Chennai     | 1000        | 410       |

## 🔧 Functionality Details

### 1. Add Train
Add new trains to the system with:
- Train Number
- Train Name
- Source Station
- Destination Station
- Total Seats

### 2. Book Ticket
- Enter train number
- Provide passenger details (name, age)
- Automatically assigns passenger ID
- If seats available: confirms booking
- If no seats: adds to waiting list

### 3. Cancel Ticket
- Cancel using passenger ID
- Automatically frees up seat
- Promotes first waiting passenger to confirmed (if any)

### 4. View Reports
- **Booked Passengers**: Lists all confirmed bookings
- **Waiting List**: Shows passengers in queue
- **Search by Train**: Filter passengers by train number
- **System Summary**: Overview of all trains and booking statistics

## 💻 Code Structure

```
├── Train Structure (Linked List)
│   ├── train_no
│   ├── train_name
│   ├── source
│   ├── destination
│   ├── total_seats
│   ├── available_seats
│   └── next
│
├── Passenger Structure (Linked List)
│   ├── id
│   ├── name
│   ├── age
│   ├── train_no
│   └── next
│
└── WaitingQueue Structure (Queue)
    ├── passenger
    └── next
```

## 🎯 Key Functions

| Function | Description |
|----------|-------------|
| `addTrain()` | Adds a new train to the system |
| `displayTrains()` | Shows all available trains |
| `searchTrain()` | Finds a train by number |
| `bookTicket()` | Books a ticket or adds to waiting list |
| `cancelTicket()` | Cancels booking and manages waiting list |
| `displayBookedPassengers()` | Shows all confirmed passengers |
| `displayWaitingList()` | Shows passengers in queue |
| `searchPassengerByTrain()` | Filters passengers by train |
| `showSystemSummary()` | Displays system statistics |

## 📝 Example Workflow

1. **Start Program** → View pre-loaded trains
2. **Book Ticket** → Enter train 20960 → Add passenger details
3. **View Booked** → See confirmed booking with passenger ID
4. **Book Again** (when full) → Passenger added to waiting list
5. **Cancel Ticket** → Enter passenger ID → Waiting passenger auto-promoted
6. **System Summary** → View complete statistics

## 🛠️ Technical Details

- **Language**: C
- **Memory Management**: Dynamic memory allocation using `malloc()`
- **Input Validation**: Basic scanf-based input
- **Data Persistence**: In-memory (data lost on exit)

## ⚠️ Limitations

- No file persistence (data is lost when program exits)
- Basic input validation
- No date/time scheduling
- Limited error handling
- Console-based interface only

## 🔮 Future Enhancements

- [ ] File-based data persistence
- [ ] Date and time scheduling
- [ ] Seat class types (AC, Sleeper, General)
- [ ] Payment integration
- [ ] GUI implementation
- [ ] Database integration
- [ ] User authentication
- [ ] Ticket printing functionality

## 👨‍💻 Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests

## 📄 License

This project is open source and available for educational purposes.

## 🙏 Acknowledgments

Built as a data structures and algorithms project demonstrating:
- Linked Lists
- Queue implementation
- Structure manipulation in C
- Dynamic memory management

---

**Happy Coding! 🚆✨**
