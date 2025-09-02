# Hotel Room Booking System

## 📌 Overview
This is a **C++ console-based hotel management system** that allows administrators to manage rooms and users to book, modify, or cancel reservations.  
The system uses **CSV files** (`rooms.csv`, `users.csv`) to persist data between sessions.

## ✨ Features
### 👨‍💼 Admin
- Add new rooms  
- Edit existing room details (type, price, location, availability)  
- Remove rooms  
- View all rooms with availability schedules  
- View all bookings for a specific room  

### 👤 User
- Sign up with a unique username & password  
- Log in securely (password masked with `*`)  
- Book a room for specific days and time periods (Morning / Afternoon / Evening)  
- View booked rooms and booking details  
- Modify existing bookings (change dates/times)  
- Cancel reservations  

### 🔑 Login
- **Admin Login**:  
  - Username: `admin`  
  - Password: `admin`  
- **User Login**: Sign up to create a new account.

## 🛠️ Requirements
- C++ compiler (e.g., **g++**, MSVC, or MinGW)  
- Console that supports `conio.h` (for password masking)  
- CSV files for data persistence:  
  - `rooms.csv` (room details & availability)  
  - `users.csv` (user accounts & bookings)

## ⚙️ Installation & Build
1. Clone or download the project.
2. Ensure you have `Source.cpp` and CSV files (`rooms.csv`, `users.csv`) in the same directory.
3. Compile the project:
   ```bash
   g++ Source.cpp -o hotel_booking
   ```
4. Run the program:
   ```bash
   ./hotel_booking
   ```

## 📂 File Structure
```
.
├── Source.cpp      # Main source code
├── rooms.csv       # Room data (auto-generated if missing)
├── users.csv       # User data (auto-generated if missing)
└── README.md       # Documentation
```

## 🗂️ CSV Format
### rooms.csv
```
ID,room type,location,price,sat morning,sat afternoon,sat evening,...
```

### users.csv
```
userID,name,username,password,numberOfBookings,bookingID,roomID,startDay,startPeriod,endDay,endPeriod,totalPrice
```

