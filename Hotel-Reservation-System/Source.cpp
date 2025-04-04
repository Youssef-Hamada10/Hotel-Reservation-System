#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
using namespace std;

//constant values.......
const int DAYS = 7;
const int PERIODS = 3;

//global values......
int USERCOUNTER = 0;
int ROOMCOUNTER = 0;
int BOOKINGCOUNTER = 0;

//structs........
struct Room {
	int roomID;
	string roomType;
	string location;
	bool availability[DAYS][PERIODS];
	int price;
};

struct Booking {
	int bookingID;
	int userID;
	int roomID;
	int startDay;
	int startPeriod;
	int endDay;
	int endPeriod;
	int totalPrice;
};

struct User {
	string name;
	int userID;
	string username;
	string password;
	vector<Booking> bookings;
	int numOfBookings = 0;
};

//functions def.......
queue<string> split(string line, char ch);
void readRooms(vector<Room>& rooms);
void writeRooms(vector<Room> rooms);
void readUsers(vector<User>& users);
void writeUsers(vector<User> users);
string replace(string str, char replaceChar, char replaceWith);
void signIn(vector<Room>& rooms, vector<User>& users);
void signUp(vector<Room>& rooms, vector<User>& users);
string usernameDomain(string name);
void adminPage(vector<Room>& rooms, vector<User> users);
string getPassword();
void addRoom(vector<Room>& rooms);
void editRoom(vector<Room>& rooms, vector<User> users);
void showAllRooms(vector<Room>& rooms);
void showBookingInfo(vector<User> users, int ID);
void userPage(vector<Room>& rooms, vector<User>& users, int currentUserID);
int findRoomById(vector<Room>& rooms, int ID);
void viewRoomBookings(vector<Room> rooms, vector<User> users, int ID); // view booking for a specific room using it's ID
void bookRoom(vector<Room>& rooms, vector<User>& users, int currentUserID);
void viewBookedRoom(vector<Room>& rooms, vector<User>& users, int currentUserID);
int encodeDays(string day);
string decodeDays(int day);
int encodePeriods(string period);
string decodePeriods(int period);
bool checkAvailability(vector<Room> rooms, int roomIndex, int startDay, int startPeriod, int endDay, int endPeriod);
void removeBooking(vector<Room>& rooms, vector<User>& users, int currentUserID);
int findBookingByID(User& user, int bookingID);
int findUserByID(vector<User>& users, int userID);
void modifyBooking(vector<Room>& rooms, vector<User>& users, int currentUserID);
void editReservationTime(vector<Room>& rooms, vector<User>& users, int currentUserID);


// functions imp......
int main() {
	// container.......
	vector<Room> rooms;
	vector<User> users;

	//read data......
	readRooms(rooms);
	readUsers(users);

	cout << "NOTICE THAT THE HOTEL IS WORKING FOR ONE WEEK ONLY...\nEACH DAY HAVE THREE PERIOD - MORNING - AFTERNON - EVENING....\n";


	int choice;
	while (true) {
		cout << "====================\n";
		cout << "Press [1] To SignIn\n";
		cout << "Press [2] To SignUp\n";
		cout << "Press [3] To Exit\n";
		cin >> choice;
		switch (choice) {
		case 1:
			signIn(rooms, users);
			break;
		case 2:
			signUp(rooms, users);
			break;
		case 3:
			// write data.....
			writeRooms(rooms);
			writeUsers(users);
			return 0;
		default:
			cout << "Incorrect Choice...\n";
			break;
		}
	}
}

// file handling fun
queue<string> split(string line, char ch) {
	int initIndex = 0;
	int finalIndex;
	queue<string> q;
	while (true) {
		finalIndex = line.find(ch);
		q.push(line.substr(initIndex, finalIndex));
		line.erase(initIndex, finalIndex + 1);
		if (finalIndex == -1)
			break;
	}
	return q;
}

void readRooms(vector<Room>& rooms) {
	Room room;
	ifstream file("rooms.csv"); // Open file for reading

	if (!file) {
		cout << "Error opening file!" << endl;
		return;
	}
	string line;
	queue<string> data;
	getline(file, line); // to ignore header
	while (getline(file, line)) {
		data = split(line, ',');
		room.roomID = stoi(data.front());
		data.pop();
		//room.roomType = replace(data.front(), '_', ' ');
		room.roomType = data.front();
		data.pop();
		//room.location = replace(data.front(), '_', ' ');
		room.location = data.front();
		data.pop();
		room.price = stoi(data.front());
		data.pop();
		for (int i = 0; i < DAYS; i++) {
			for (int j = 0; j < PERIODS; j++) {
				room.availability[i][j] = (data.front() == "1");
				data.pop();
			}
		}
		rooms.push_back(room);
		ROOMCOUNTER++;
	}
	file.close();
}

void writeRooms(vector<Room> rooms) {
	ofstream file("rooms.csv"); // Open file for writing

	if (!file) {
		cout << "Error opening file!" << endl;
		return;
	}

	file << "ID,room type,location,price, ,sat, , ,sun, , ,mon, , ,tue, , ,wed, , ,thu, , ,fri, \n";  // Header
	for (Room room : rooms) {
		file << room.roomID;
		file << "," << room.roomType;
		//file << "," << replace(room.location, ' ', '_');
		file << "," << room.location;
		file << "," << room.price;
		for (int i = 0; i < DAYS; i++) {
			for (int j = 0; j < PERIODS; j++) {
				file << "," << room.availability[i][j];
			}
		}
		file << "\n";
	}
	file.close(); // Close the file
}

void readUsers(vector<User>& users) {
	User user;
	Booking booking;
	ifstream file("users.csv"); // Open file for reading

	if (!file) {
		cout << "Error opening file!" << endl;
		return;
	}
	string line;
	queue<string> data;
	getline(file, line); // to ignore header
	while (getline(file, line)) {
		data = split(line, ',');
		user.userID = stoi(data.front());
		data.pop();
		//user.name = replace(data.front(), '_', ' ');
		user.name = data.front();
		data.pop();
		user.username = data.front();
		data.pop();
		user.password = data.front();
		data.pop();
		int counter = user.numOfBookings = stoi(data.front());
		data.pop();
		while (counter > 0) {
			booking.bookingID = stoi(data.front());
			data.pop();
			booking.userID = user.userID;
			booking.roomID = stoi(data.front());
			data.pop();
			booking.startDay = stoi(data.front());
			data.pop();
			booking.startPeriod = stoi(data.front());
			data.pop();
			booking.endDay = stoi(data.front());
			data.pop();
			booking.endPeriod = stoi(data.front());
			data.pop();
			booking.totalPrice = stoi(data.front());
			data.pop();
			user.bookings.push_back(booking);
			BOOKINGCOUNTER++;
			counter--;
		}
		users.push_back(user);
		USERCOUNTER++;
	}
	file.close();
}

void writeUsers(vector<User> users) {
	ofstream file("users.csv"); // Open file for writing

	if (!file) {
		cout << "Error opening file!" << endl;
		return;
	}

	file << "userID,name,username,password,numberOfBookings,bookingID,roomID,startDay,startPeriod,endDay,endPeriod,totalPrice \n";  // Header
	for (User user : users) {
		file << user.userID << ",";
		//file << replace(user.name, ' ', '_') << ",";
		file << user.name << ",";
		file << user.username << ",";
		file << user.password << ",";
		file << user.numOfBookings;
		for (Booking booking : user.bookings) {
			file << "," << booking.bookingID << ",";
			file << booking.roomID << ",";
			file << booking.startDay << ",";
			file << booking.startPeriod << ",";
			file << booking.endDay << ",";
			file << booking.endPeriod << ",";
			file << booking.totalPrice;
		}
		file << "\n";
	}
	file.close(); // Close the file
}

string replace(string str, char replaceChar, char replaceWith) {
	string replaced = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == replaceChar) {
			replaced += replaceWith;
		}
		else {
			replaced += str[i];
		}
	}
	return replaced;
}

// signIn fun
void signIn(vector<Room>& rooms, vector<User>& users) {
	cout << "====================\n";
	bool valid = false; // to check validation
	cout << "Enter Your Username:\n";
	string username;
	cin >> username;
	cout << "Enter Your Password:\n";
	string password = getPassword();
	if (username == "admin" && password == "admin") {
		valid = true;
		adminPage(rooms, users);
	}
	else {
		for (User user : users) {
			if (username == user.username && password == user.password) {
				valid = true;
				userPage(rooms, users, user.userID);
				break;
			}
		}
	}
	if (!valid) {
		cout << "Incorrect Username or Password...\n";
	}
}

// signUp fun
void signUp(vector<Room>& rooms, vector<User>& users) {
	cout << "====================\n";
	bool valid = false;
	User temp;
	cout << "Enter Your Full Name:\n";
	getline(cin.ignore(1, '\n'), temp.name);
	temp.username = usernameDomain(temp.name);
	cout << "Your Username Is : " << temp.username << endl;
	cout << "Enter Your Password:\n";
	temp.password = getPassword();
	temp.userID = ++USERCOUNTER;
	cout << "You Have Signed Up...\n";
	users.push_back(temp);
	userPage(rooms, users, temp.userID);
}

string usernameDomain(string name) {
	string username = name;
	username[username.find(' ')] = '-';
	username.append("@User.Hotel");
	return username;
}

// adminPage fun
void adminPage(vector<Room>& rooms, vector<User> users) {
	cout << "====================\n";
	cout << "WELCOME, ADMIN\n";
	int choice;
	while (true) {
		cout << "====================\n";
		cout << "Press [1] To Add Room:\n";
		cout << "Press [2] To Edit Rooms:\n";
		cout << "Press [3] To Show All Rooms:\n";
		cout << "Press [4] to view room Bookings\n";
		cout << "Press [5] To Exit:\n";
		cin >> choice;
		switch (choice) {
		case 1:
			addRoom(rooms);
			break;
		case 2:
			editRoom(rooms, users);
			break;
		case 3:
			showAllRooms(rooms);
			break;
		case 4:
			if (rooms.empty()){
				cout << "There IS no Room Yet...\n";
				return;
			}
			cout << "Enter Room ID To Show It's Bookings\n";
			int ID;
			cin >> ID;
			viewRoomBookings(rooms, users, ID);
			break;
		case 5:
			return;
		default:
			cout << "Incorrect choice...\n";
			break;
		}
	}
}

// password as astrisk fun
string getPassword() {
	string password;
	char ch;
	while (true) {
		ch = _getch();
		if (ch == 13) {
			// enter bottom
			if (!password.empty()) {
				cout << endl;
				return password;
			}
			else
				continue;
		}
		else if ((ch == 8) && (!password.empty())) {
			// delete bottom
			cout << "\b \b";
			password.pop_back();
		}
		else if (ch != 8) {
			password.push_back(ch);
			cout << "*";
		}
	}
}

// addRoom fun
void addRoom(vector<Room>& rooms) {
	cout << "====================\n";
	Room temp;
	temp.roomID = ++ROOMCOUNTER;
	cout << "Enter Room Type:\n";
	getline(cin.ignore(1, '\n'), temp.roomType);
	cout << "Enter Room Price Per Period:\n";
	cin >> temp.price;
	cout << "Enter Room Location:\n";
	getline(cin.ignore(1, '\n'), temp.location);
	for (int day = 0; day < DAYS; day++) { // make the room available all days
		for (int period = 0; period < PERIODS; period++) {
			temp.availability[day][period] = 1;
		}
	}
	cout << "Room Added...\n";
	rooms.push_back(temp);
}

// editRoom fun
void editRoom(vector<Room>& rooms, vector<User> users) {
	showAllRooms(rooms);
	if (rooms.empty())
		return;
	cout << "Enter Room ID to Edit It:\n";
	int ID;
	cin >> ID;
	int choice;
	while (true) {
		cout << "====================\n";
		cout << "Press [1] To Edit Room Type\n";
		cout << "Press [2] to Edit Room Price\n";
		cout << "Press [3] to Edit Room Location\n";
		cout << "Press [4] to Edit Room Availability\n";
		cout << "Press [5] to Remove Room\n";
		cout << "Press [6] to Exit\n";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "The Current Type Is: " << rooms[findRoomById(rooms, ID)].roomType << "...Enter The New Type:\n";
			getline(cin.ignore(1, '\n'), rooms[findRoomById(rooms, ID)].roomType);
			break;
		case 2:
			cout << "The Price Type Is: " << rooms[findRoomById(rooms, ID)].price << "...Enter The New Price:\n";
			cin >> rooms[findRoomById(rooms, ID)].price;
			break;
		case 3:
			cout << "The Current Location Is: " << rooms[findRoomById(rooms, ID)].location << "...Enter The New Location:\n";
			getline(cin.ignore(1, '\n'), rooms[findRoomById(rooms, ID)].location);
			break;
		case 4:
			cout << "Enter Room Availability:\n";
			for (int day = 0; day < DAYS; day++) {
				cout << "\tFor " << decodeDays(day) << endl;
				for (int period = 0; period < PERIODS; period++) {
					cout << "For " << decodePeriods(period)  << " : ";
					cin >> rooms[findRoomById(rooms, ID)].availability[day][period];
				}
			}
			break;
		case 5:
			rooms.erase(rooms.begin() + findRoomById(rooms, ID), rooms.begin() + findRoomById(rooms, ID) + 1);
			cout << "Room Is Removed.....\n";
			break;
		case 6:
			return;
		default:
			cout << "Incorrect choice...\n";
			break;
		}
	}
}

// showAllRooms fun
void showAllRooms(vector<Room>& rooms) {
	cout << "====================\n";
	if (rooms.empty()) {
		cout << "There Is No Rooms To Show\n";
		return;
	}
	else {
		for (Room room : rooms) {
			printf("RoomID Is: %-3i Room Price Is: %-10i", room.roomID, room.price);
			cout << "Room Type Is: " << room.roomType << "\t Room Location Is: " << room.location << "\n";
			cout << "\n";
			cout << "\t\tSat\tSun\tMon\tTue\tWed\tThu\tFri\n";
			for (int period = 0; period < PERIODS; period++) {
				if (period == 0)
					cout << "Morning ";
				else if (period == 1)
					cout << "Afternon";
				else
					cout << "Evening ";
				for (int day = 0; day < DAYS; day++) {
					printf("\t%-7d", room.availability[day][period]);
				}
				cout << "\n";
			}
			cout << "====================\n";
		}
	}
}

// viewRoombookings fun
void viewRoomBookings(vector<Room> rooms, vector<User> users, int ID) {
	cout << "====================\n";
	if (rooms.empty()) {
		cout << "There IS No Room To Show\n";
		return;
	}
	else {
		int counter = 0;
		for (int i = 0; i < users.size(); i++) {
			for (int j = 0; j < users[i].bookings.size(); j++) {
				if (ID == users[i].bookings[j].roomID) {
					counter++;
				}
			}
		}
		if (counter > 0) {
			cout << "This Room Has " << counter << " Bookings\n";
			showBookingInfo(users, ID);
		}
		else {
			cout << "This Room Has No Bookings\n";
		}
	}
}

// showBookingInfo using room ID fun
void showBookingInfo(vector<User> users, int ID) {
	cout << "====================\n";
	for (int i = 0; i < users.size(); i++) {
		for (int j = 0; j < users[i].bookings.size(); j++) {
			if (ID == users[i].bookings[j].roomID) {
				cout << "Booking ID Is: " << users[i].bookings[j].bookingID;
				cout << "\t User ID Is: " << users[i].bookings[j].userID;
				cout << "\t Room ID Is: " << users[i].bookings[j].roomID;
				cout << "\t Start Day Is: " << users[i].bookings[j].startDay;
				cout << "\t Start Period Is: " << users[i].bookings[j].startPeriod;
				cout << "\t End Day Is: " << users[i].bookings[j].endDay;
				cout << "\t End Period Is: " << users[i].bookings[j].endPeriod;
				cout << endl;
			}
		}
	}
}

// userPage fun
void userPage(vector<Room>& rooms, vector<User>& users, int currentUserID) {
	cout << "====================\n";
	cout << "WELCOME, " << users[currentUserID - 1].name << endl;
	int choice;
	while (true) {
		cout << "====================\n";
		cout << "Press [1] To Book Room:\n";
		cout << "Press [2] To View Booked Room:\n";
		cout << "Press [3] To Modify Booking:\n";
		cout << "Press [4] To Exit:\n";
		cin >> choice;
		switch (choice) {
		case 1:
			bookRoom(rooms, users, currentUserID);
			break;
		case 2:
			viewBookedRoom(rooms, users, currentUserID);
			break;
		case 3:
			modifyBooking(rooms, users, currentUserID);
			break;
		case 4:
			return;
		default:
			cout << "Incorrect choice...\n";
			break;
		}
	}
}

// bookRoom fun
void bookRoom(vector<Room>& rooms, vector<User>& users, int currentUserID) {
	cout << "====================\n";
	bool valid = false;
	string sd, ed, sp, ep;
	int ID, roomIndex, startDay, startPeriod, endDay, endPeriod;
	if (rooms.empty()) {
		cout << "There Is not Any Room Exist...\n";
		return;
	}
	else {
		cout << "NOTICE THAT IF YOU BOOK A PERIOD IN ANY DAY YOU WILL PAY FOR THE WHOLE DAY \n\n";
		showAllRooms(rooms);
		cout << "Enter Room ID to Book It:\n";
		cin >> ID;
		roomIndex = findRoomById(rooms, ID);
		while (!valid) {
			cout << "Enter Start day:\n";
			cin >> sd;
			cout << "Enter Start period:\n";
			cin >> sp;
			cout << "Enter End Day:\n";
			cin >> ed;
			cout << "Enter End Period:\n";
			cin >> ep;
			startDay = encodeDays(sd);
			startPeriod = encodePeriods(sp);
			endDay = encodeDays(ed);
			endPeriod = encodePeriods(ep);
			valid = checkAvailability(rooms, roomIndex, startDay, startPeriod, endDay, endPeriod);
			if (!valid)
				cout << "The Room May Be Has Been Booked...\n";
		}
		for (int day = startDay; day <= endDay; day++) {
			for (int period = 0; period < PERIODS; period++) {
				if ((startDay == day && startPeriod <= period) || (startDay < day && endDay > day) || (endDay == day && endPeriod >= period)) {
					rooms[roomIndex].availability[day][period] = false;
				}
				else
					continue;
			}
		}
		Booking booking;
		booking.bookingID = ++BOOKINGCOUNTER;
		booking.roomID = ID;
		booking.userID = currentUserID;
		booking.startDay = startDay;
		booking.startPeriod = startPeriod;
		booking.endDay = endDay;
		booking.endPeriod = endPeriod;
		booking.totalPrice = (endDay - startDay + 1) * rooms[roomIndex].price;
		users[currentUserID - 1].bookings.push_back(booking);
		users[currentUserID - 1].numOfBookings++;
		cout << "The Room Is Booked Successfully......\n";
	}
}

// days fun
int encodeDays(string day) {
	if (day == "Sat" || day == "sat")
		return 0;
	else if (day == "Sun" || day == "sun")
		return 1;
	else if (day == "Mon" || day == "mon")
		return 2;
	else if (day == "Tue" || day == "tue")
		return 3;
	else if (day == "Wed" || day == "wed")
		return 4;
	else if (day == "Thu" || day == "thu")
		return 5;
	else if (day == "Fri" || day == "fri")
		return 6;
	else
		return -1;
}
string decodeDays(int day) {
	switch (day) {
	case 0:
		return "sat";
		break;
	case 1:
		return "sun";
		break;
	case 2:
		return "mon";
		break;
	case 3:
		return "tue";
		break;
	case 4:
		return "wed";
		break;
	case 5:
		return "thu";
		break;
	case 6:
		return "fri";
		break;
	}
}

// periods fun
int encodePeriods(string period) {
	if (period == "morning" || period == "Morning")
		return 0;
	else if (period == "Afternon" || period == "afternon")
		return 1;
	else if (period == "Evening" || period == "evening")
		return 2;
	else
		return -1;
}
string decodePeriods(int period) {
	switch (period) {
	case 0:
		return "morning ";
		break;
	case 1:
		return "afternon";
		break;
	case 2:
		return "evening ";
		break;
	}
}

// checkAvailability fun
bool checkAvailability(vector<Room> rooms, int roomIndex, int startDay, int startPeriod, int endDay, int endPeriod) {
	bool avail = true;
	for (int day = startDay; day <= endDay; day++) {
		for (int period = 0; period < PERIODS; period++) {
			if ((day == startDay && startPeriod <= period) || (day > startDay && day < endDay) || (startDay == endDay && endPeriod >= period)) {
				if (rooms[roomIndex].availability[day][period] == false) {
					avail = false;
					break;
				}
			}
			else
				continue;
		}
		if (!avail) {
			break;
		}
	}
	return avail;
}

// viewBookedRoom fun
void viewBookedRoom(vector<Room>& rooms, vector<User>& users, int currentUserID) {
	cout << "=================\n";
	if (users[currentUserID - 1].bookings.empty()) {
		cout << "You Have Not Any Bookings Yet...\n";
	}
	else {
		for (Booking booking : users[currentUserID - 1].bookings) {
			cout << "Booking ID Is: " << booking.bookingID;
			cout << "\t Room ID Is: " << booking.roomID;
			cout << "\t Start Day Is: " << booking.startDay;
			cout << "\t Start Period Is: " << booking.startPeriod;
			cout << "\t End Day Is: " << booking.endDay;
			cout << "\t End Period Is: " << booking.endPeriod;
			cout << "\t Total Price Is: " << booking.totalPrice << endl;
		}
	}
}

// modifyBooking fun
void modifyBooking(vector<Room>& rooms, vector<User>& users, int currentUserID) {
	cout << "====================\n";
	int choice;
	while (true) {
		cout << "Press [1] To Remove Booking:\n";
		cout << "Press [2] To Edit Reservation Time:\n";
		cout << "Press [3] To Exit:\n";
		cin >> choice;
		switch (choice) {
		case 1:
			removeBooking(rooms, users, currentUserID);
			break;
		case 2:
			editReservationTime(rooms, users, currentUserID);
			break;
		case 3:
			return;
		default:
			cout << "Incorrect choice...\n";
			break;
		}
	}
}

// removeBooking fun 
void removeBooking(vector<Room>& rooms, vector<User>& users, int currentUserID) {
	cout << "====================\n";
	int bookingID;
	if (users[currentUserID - 1].bookings.empty()) {
		cout << "You Have Not Any Bookings Yet...\n";
		return;
	}
	else {
		viewBookedRoom(rooms, users, currentUserID);
		cout << "====================\n";
		cout << "Enter Booking ID That You Want To Remove:\n";
		cin >> bookingID;
		for (int i = 0; i < users[currentUserID - 1].bookings.size(); i++) {
			if (bookingID  == users[currentUserID - 1].bookings[i].bookingID) {
				for (int day = users[currentUserID - 1].bookings[i].startDay; day <= users[currentUserID - 1].bookings[i].endDay; day++) {
					for (int period = 0; period < PERIODS; period++) {
						if ((users[currentUserID - 1].bookings[i].startDay == day && users[currentUserID - 1].bookings[i].startPeriod <= period) || (users[currentUserID - 1].bookings[i].startDay < day && users[currentUserID - 1].bookings[i].endDay > day) || (users[currentUserID - 1].bookings[i].endDay == day && users[currentUserID - 1].bookings[i].endPeriod >= period)) {
							rooms[users[currentUserID - 1].bookings[i].roomID - 1].availability[day][period] = true;
						}
						else
							continue;
					}
				}
				break;
			}
		}
		BOOKINGCOUNTER--;
		users[currentUserID - 1].bookings.erase(users[currentUserID - 1].bookings.begin() + findBookingByID(users[currentUserID - 1], bookingID), users[currentUserID - 1].bookings.begin() + findBookingByID(users[currentUserID - 1], bookingID) + 1);
		users[currentUserID - 1].numOfBookings--;
		cout << "The Booking Is Removed Successfully\n";
	}
}

// findRoomById fun
int findRoomById(vector<Room>& rooms, int ID) { // return the index of the room
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomID == ID) {
			return i;
			break;
		}
	}
}

// findBookingByID fun
int findBookingByID(User& user, int bookingID) {
	for (int i = 0; i < user.bookings.size(); i++) {
		if (bookingID == user.bookings[i].bookingID) {
			return i;
			break;
		}
	}
}

// findUserByID fun
int findUserByID(vector<User>& users, int userID) {
	for (int i = 0; i < users.size(); i++) {
		if (userID == users[i].userID) {
			return i;
			break;
		}
	}
}

// editReservationTime fun
void editReservationTime(vector<Room>& rooms, vector<User>& users, int currentUserID) {
	cout << "====================\n";
	if (users[currentUserID - 1].bookings.empty()) {
		cout << "You Have Not Any Bookings Yet...\n";
		return;
	}
	else {
		string input;
		int bookingID;
		int choice;
		viewBookedRoom(rooms, users, currentUserID);
		cout << "====================\n";
		cout << "Enter Booking ID That You Want To Edit It:\n";
		cin >> bookingID;
		while (true) {
			cout << "Press [1] To Edit Start Day:\n";
			cout << "Press [2] To Edit Start Period:\n";
			cout << "Press [3] To Edit End Day:\n";
			cout << "Press [4] To Edit End Period:\n";
			cout << "Press [5] To Exit:\n";
			cin >> choice;
			switch (choice) {
			case 1:
				cout << "Enter The New Start Day\n";
				cin >> input;
				if (encodeDays(input) > users[currentUserID - 1].bookings[bookingID - 1].endDay) {
					cout << "Can Not Edit The Start Day\n";
				}
				else {
					users[currentUserID - 1].bookings[bookingID - 1].startDay = encodeDays(input);
					users[currentUserID - 1].bookings[bookingID - 1].totalPrice = (users[currentUserID - 1].bookings[bookingID - 1].endDay - encodeDays(input) + 1) * rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].price;
					for (int day = users[currentUserID - 1].bookings[bookingID - 1].startDay; day <= users[currentUserID - 1].bookings[bookingID - 1].endDay; day++) {
						for (int period = users[currentUserID - 1].bookings[bookingID - 1].startPeriod; period < users[currentUserID - 1].bookings[bookingID - 1].endPeriod; period++) {
							if ((users[currentUserID - 1].bookings[bookingID - 1].startDay == day && users[currentUserID - 1].bookings[bookingID - 1].startPeriod <= period) || (users[currentUserID - 1].bookings[bookingID - 1].startDay < day && users[currentUserID - 1].bookings[bookingID - 1].endDay > day) || (users[currentUserID - 1].bookings[bookingID - 1].endDay == day && users[currentUserID - 1].bookings[bookingID - 1].endPeriod >= period)) {
								rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[day][period] = true;
							}
							else
								continue;
						}
					}

					for (int day = encodeDays(input); day <= users[currentUserID - 1].bookings[bookingID - 1].endDay; day++) {
						for (int period = users[currentUserID - 1].bookings[bookingID - 1].startPeriod; period < users[currentUserID - 1].bookings[bookingID - 1].endPeriod; period++) {
							if ((users[currentUserID - 1].bookings[bookingID - 1].startDay == day && users[currentUserID - 1].bookings[bookingID - 1].startPeriod <= period) || (users[currentUserID - 1].bookings[bookingID - 1].startDay < day && users[currentUserID - 1].bookings[bookingID - 1].endDay > day) || (users[currentUserID - 1].bookings[bookingID - 1].endDay == day && users[currentUserID - 1].bookings[bookingID - 1].endPeriod >= period)) {
								rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[day][period] = false;
							}
							else
								continue;
						}
					}
					cout << "The Start Day Was Updated Successfully...\n";
				}
				break;
			case 2:
				cout << "Enter The New Start Period\n";
				cin >> input;
				users[currentUserID - 1].bookings[bookingID - 1].startPeriod = encodePeriods(input);
				if (encodePeriods(input) < users[currentUserID - 1].bookings[bookingID - 1].startPeriod) {
					for (int i = encodePeriods(input); i < users[currentUserID - 1].bookings[bookingID - 1].startPeriod; i++) {
						rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[users[currentUserID - 1].bookings[bookingID - 1].startDay][i] = false;
					}
				}
				else {
					for (int i = users[currentUserID - 1].bookings[bookingID - 1].startPeriod; i < encodePeriods(input); i++) {
						rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[users[currentUserID - 1].bookings[bookingID - 1].startDay][i] = true;
					}
				}
				cout << "The Start Period Was Updated Successfully...\n";
				break;
			case 3:
				cout << "Enter The New End Day\n";
				cin >> input;
				if (encodeDays(input) < users[currentUserID - 1].bookings[bookingID - 1].startDay) {
					cout << "Can Not Edit The End Day\n";
				}
				else {
					users[currentUserID - 1].bookings[bookingID - 1].endDay = encodeDays(input);
					users[currentUserID - 1].bookings[bookingID - 1].totalPrice = (encodeDays(input) - users[currentUserID - 1].bookings[bookingID - 1].startDay + 1) * rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].price;
					for (int day = users[currentUserID - 1].bookings[bookingID - 1].startDay; day <= users[currentUserID - 1].bookings[bookingID - 1].endDay; day++) {
						for (int period = users[currentUserID - 1].bookings[bookingID - 1].startPeriod; period < users[currentUserID - 1].bookings[bookingID - 1].endPeriod; period++) {
							if ((users[currentUserID].bookings[bookingID - 1].startDay == day && users[currentUserID].bookings[bookingID - 1].startPeriod <= period) || (users[currentUserID - 1].bookings[bookingID - 1].startDay < day && users[currentUserID - 1].bookings[bookingID - 1].endDay > day) || (users[currentUserID - 1].bookings[bookingID - 1].endDay == day && users[currentUserID - 1].bookings[bookingID - 1].endPeriod >= period)) {
								rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[day][period] = true;
							}
							else
								continue;
						}
					}

					for (int day = users[currentUserID - 1].bookings[bookingID - 1].startDay; day <= encodeDays(input); day++) {
						for (int period = users[currentUserID - 1].bookings[bookingID - 1].startPeriod; period < users[currentUserID - 1].bookings[bookingID - 1].endPeriod; period++) {
							if ((users[currentUserID - 1].bookings[bookingID - 1].startDay == day && users[currentUserID - 1].bookings[bookingID - 1].startPeriod <= period) || (users[currentUserID- 1].bookings[bookingID - 1].startDay < day && users[currentUserID - 1].bookings[bookingID - 1].endDay > day) || (users[currentUserID - 1].bookings[bookingID - 1].endDay == day && users[currentUserID - 1].bookings[bookingID - 1].endPeriod >= period)) {
								rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[day][period] = false;
							}
							else
								continue;
						}
					}
					cout << "The End Day Was Updated Successfully...\n";
				}
				break;;
			case 4:
				cout << "Enter The New End Period\n";
				cin >> input;
				users[currentUserID - 1].bookings[bookingID - 1].endPeriod = encodePeriods(input);
				if (encodePeriods(input) < users[currentUserID - 1].bookings[bookingID - 1].endPeriod) {
					for (int i = encodePeriods(input) + 1; i <= users[currentUserID - 1].bookings[bookingID - 1].startPeriod; i++) {
						rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[users[currentUserID - 1].bookings[bookingID - 1].endDay][i] = true;
					}
				}
				else {
					for (int i = users[currentUserID - 1].bookings[bookingID - 1].endPeriod + 1; i <= encodePeriods(input); i++) {
						rooms[findRoomById(rooms, users[currentUserID - 1].bookings[bookingID - 1].roomID)].availability[users[currentUserID - 1].bookings[bookingID - 1].endDay][i] = false;
					}
				}
				cout << "The End Period Was Updated Successfully...\n";
				break;
			case 5:
				return;
			default:
				cout << "Incorrect choice...\n";
				break;
			}
		}
	}
}