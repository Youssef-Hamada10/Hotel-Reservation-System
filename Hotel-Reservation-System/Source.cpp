#include <iostream>
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
void signUp(vector<User>& users);
void adminPage(vector<Room>& rooms, vector<User> users);
void addRoom(vector<Room>& rooms);
void editRoom(vector<Room>& rooms, vector<User> users);
void showAllRooms(vector<Room>& rooms);
void showBookingInfo(vector<User> users, int ID);
void userPage(vector<Room>& rooms, vector<User>& users, User& user);
int findRoomById(vector<Room>& rooms, int ID);
void viewRoomBookings(vector<Room> rooms, vector<User> users, int ID); // view booking for a specific room using it's ID
void bookRoom(vector<Room>& rooms, vector<User>& users, User& user);
void viewBookedRoom(vector<Room>& rooms, User user);
int encodeDays(string day);
string decodeDays(int day);
int encodePeriods(string period);
string decodePeriods(int period);
bool checkAvailability(vector<Room> rooms, int roomIndex, int startDay, int startPeriod, int endDay, int endPeriod);
//void viewBookedRoom(User user);
void removeBooking(vector<Room>& rooms, vector<User>& users, User user);
int findBookingByID(User user, int bookingID);
int findUserByID(vector<User>& users, int userID);
void modifyBooking(vector<Room>& rooms, vector<User>& users, User user);
void editReservationTime(vector<Room>& rooms, vector<User>& users, User user);


// functions imp......
void main() {
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
			signUp(users);
			break;
		case 3:
			// write data.....
			writeRooms(rooms);
			writeUsers(users);
			return;
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
		room.roomType = replace(data.front(), '_', ' ');
		data.pop();
		room.location = replace(data.front(), '_', ' ');
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

	file << "ID,room type,location,price,sat, , ,sun, , ,mon, , ,tue, , ,wed, , ,thu, , ,fri, , \n";  // Header
	for (Room room : rooms) {
		file << room.roomID;
		file << "," << room.roomType;
		file << "," << replace(room.location, ' ', '_');
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
		user.name = replace(data.front(), '_', ' ');
		data.pop();
		user.username = data.front();
		data.pop();
		user.numOfBookings = stoi(data.front());
		data.pop();
		int counter = user.numOfBookings;
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

	file << "userID,name,username,password,numberOfBookings,bookingID,roomID,startDay,startPeriod,endDay,endPeriod \n";  // Header
	for (User user : users) {
		file << user.userID << " " << replace(user.name, ' ', '_') << " " << user.username << " " << user.password << " " << user.numOfBookings;
		for (Booking booking : user.bookings) {
			file << booking.bookingID << " " << booking.roomID << " " << booking.startDay << " " << booking.startPeriod;
			file << " " << booking.endDay << " " << booking.endPeriod;
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
	string password;
	cin >> password;
	if (username == "admin" && password == "admin") {
		valid = true;
		adminPage(rooms, users);
	}
	else {
		for (User user : users) {
			if (username == user.username && password == user.password) {
				valid = true;
				userPage(rooms, users, user);
				break;
			}
		}
	}
	if (!valid) {
		cout << "Incorrect Username or Password...\n";
	}
}

// signUp fun
void signUp(vector<User>& users) {
	cout << "====================\n";
	bool valid = false;
	User temp;
	cout << "Enter Your Name:\n";
	getline(cin.ignore(1, '\n'), temp.name);
	while (!valid) {
		cout << "Enter Your Username:\n";
		cin >> temp.username;
		valid = true;
		for (User user : users) {
			if (user.username == temp.username) {
				cout << "The Username is Already Exist...\n";
				valid = false;
				break;
			}
		}
	}
	cout << "Enter Your Password:\n";
	cin >> temp.password;
	temp.userID = ++USERCOUNTER;
	cout << "You Have Signed Up...\n";
	users.push_back(temp);

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
		cout << "Press [4] To Exit:\n";
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
			return;
		default:
			cout << "Incorrect choice...\n";
			break;
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
	cout << "Enter Room Price:\n";
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
		cout << "Press [5] to view room Bookings\n";
		cout << "Press [6] to Remove Room\n";
		cout << "Press [7] to Exit\n";
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
				cout << "For Day " << day + 1;
				for (int period = 0; period < PERIODS; period++) {
					cout << "For Period " << period + 1;
					cin >> rooms[findRoomById(rooms, ID)].availability[day][period];
				}
			}
			break;
		case 5:
			viewRoomBookings(rooms, users, ID);
			break;
		case 6:
			rooms.erase(rooms.begin() + findRoomById(rooms, ID), rooms.begin() + findRoomById(rooms, ID) + 1);
			cout << "Room Is Removed.....\n";
			break;
		case 7:
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
			cout << "\t\tSat\tSun\tMon\tTus\tWed\tThu\tFri\n";
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
				printf("Booking ID Is: %-3i User ID Is: %-3i Room ID Is %-3i ", users[i].bookings[j].bookingID, users[i].bookings[j].userID, users[i].bookings[j].roomID);
				cout << "Start Day Is: " << decodeDays(users[i].bookings[j].startDay) << "   Start Period Is: " << decodePeriods(users[i].bookings[j].startPeriod) << "   End Day Is: " << decodeDays(users[i].bookings[j].endDay) << "   End Period Is: " << decodePeriods(users[i].bookings[j].endPeriod);
				cout << endl;
			}
		}
	}
}

// userPage fun
void userPage(vector<Room>& rooms, vector<User>& users, User& user) {
	cout << "====================\n";
	cout << "WELCOME, " << user.name << endl;
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
			bookRoom(rooms, users, user);
			break;
		case 2:
			viewBookedRoom(rooms, user);
			break;
		case 3:
			modifyBooking(rooms, users, user);
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
void bookRoom(vector<Room>& rooms, vector<User>& users, User& user) {
	cout << "====================\n";
	bool valid = false;
	string sd, ed, sp, ep;
	int ID, roomIndex, startDay, startPeriod, endDay, endPeriod;
	if (rooms.empty()) {
		cout << "There Is not Any Room Exist...\n";
		return;
	}
	else {
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
		booking.userID = user.userID;
		booking.startDay = startDay;
		booking.startPeriod = startPeriod;
		booking.endDay = endDay;
		booking.endPeriod = endPeriod;
		//users[findUserByID(users, user.userID)].bookings.push_back(booking);
		//users[findUserByID(users, user.userID)].numOfBookings++;
		user.bookings.push_back(booking);
		user.numOfBookings++;
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
	else if (day == "Tus" || day == "tus")
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
			if ((day == startDay && startPeriod >= period) || (day > startDay && day < endDay) || (startDay == endDay && endPeriod >= period)) {
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
void viewBookedRoom(vector<Room>& rooms, User user) {
	if (user.bookings.empty()) {
		cout << "You Have Not Any Bookings Yet...\n";
	}
	else {
		for (Booking booking : user.bookings) {
			printf("Booking ID Is: %-3i User ID Is: %-3i Room ID Is %-3i ", booking.bookingID, booking.userID, booking.roomID);
			cout << "Room Location Is : " << rooms[findRoomById(rooms, booking.roomID)].location << endl;
			cout << "Start Day Is: " << decodeDays(booking.startDay) << "   Start Period Is: " << decodePeriods(booking.startPeriod) << "   End Day Is: " << decodeDays(booking.endDay) << "   End Period Is: " << decodePeriods(booking.endPeriod);
			cout << endl;
		}
	}
}

// modifyBooking fun
void modifyBooking(vector<Room>& rooms, vector<User>& users, User user) {
	cout << "====================\n";
	int choice;
	while (true) {
		cout << "Press [1] To Remove Booking:\n";
		cout << "Press [2] To Edit Reservation Time:\n";
		cout << "Press [3] To Exit:\n";
		cin >> choice;
		switch (choice) {
		case 1:
			removeBooking(rooms, users, user);
			break;
		case 2:
			editReservationTime(rooms, users, user);
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
void removeBooking(vector<Room>& rooms, vector<User>& users, User user) {
	cout << "====================\n";
	int startDay, startPeriod, endDay, endPeriod, bookingID, roomID;
	if (user.bookings.empty()) {
		cout << "You Have Not Any Bookings Yet...\n";
		return;
	}
	else {
		viewBookedRoom(rooms, user);
		cout << "====================\n";
		cout << "Enter Booking ID That You Want To Remove:\n";
		cin >> bookingID;
		for (int i = 0; i < user.bookings.size(); i++) {
			if (bookingID == user.bookings[i].bookingID) {
				startDay = user.bookings[i].startDay;
				startPeriod = user.bookings[i].startPeriod;
				endDay = user.bookings[i].endDay;
				endPeriod = user.bookings[i].endPeriod;
				roomID = user.bookings[i].roomID;
				break;
			}
		}
		for (int day = startDay; day <= endDay; day++) {
			for (int period = 0; period < PERIODS; period++) {
				if ((startDay == day && startPeriod <= period) || (startDay < day && endDay > day) || (endDay == day && endPeriod >= period)) {
					rooms[roomID].availability[day][period] = true;
				}
				else
					continue;
			}
		}
		BOOKINGCOUNTER--;
		//users[findUserByID(users, user.userID)].bookings.erase(user.bookings.begin() + findBookingByID(user, bookingID), user.bookings.begin() + findBookingByID(user, bookingID) + 1);
		user.bookings.erase(user.bookings.begin() + findBookingByID(user, bookingID), user.bookings.begin() + findBookingByID(user, bookingID) + 1);
		user.numOfBookings--;
		//users[findUserByID(users, user.userID)].numOfBookings--;
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
int findBookingByID(User user, int bookingID) {
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
void editReservationTime(vector<Room>& rooms, vector<User>& users, User user) {
	cout << "====================\n";
	if (user.bookings.empty()) {
		cout << "You Have Not Any Bookings Yet...\n";
		return;
	}
	else {
		string input;
		int bookingID;
		int choice;
		viewBookedRoom(rooms, user);
		cout << "====================\n";
		cout << "Enter Booking ID That You Want To Remove:\n";
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
				if (encodeDays(input) > user.bookings[bookingID].endDay) {
					cout << "Can Not Edit The Start Day\n";
				}
				else {
					users[findUserByID(users, user.userID)].bookings[bookingID].startDay = encodeDays(input);
					user.bookings[bookingID].startDay = encodeDays(input);
					for (int day = user.bookings[bookingID].startDay; day <= user.bookings[bookingID].endDay; day++) {
						for (int period = 0; period < PERIODS; period++) {
							if ((user.bookings[bookingID].startDay == day && user.bookings[bookingID].startPeriod <= period) || (user.bookings[bookingID].startDay < day && user.bookings[bookingID].endDay > day) || (user.bookings[bookingID].endDay == day && user.bookings[bookingID].endPeriod >= period)) {
								rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[day][period] = true;
							}
							else
								continue;
						}
					}
					for (int day = encodeDays(input); day <= user.bookings[bookingID].endDay; day++) {
						for (int period = 0; period < PERIODS; period++) {
							if ((user.bookings[bookingID].startDay == day && user.bookings[bookingID].startPeriod <= period) || (user.bookings[bookingID].startDay < day && user.bookings[bookingID].endDay > day) || (user.bookings[bookingID].endDay == day && user.bookings[bookingID].endPeriod >= period)) {
								rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[day][period] = false;
							}
							else
								continue;
						}
					}
				}
				break;
			case 2:
				cout << "Enter The New Start Period\n";
				cin >> input;
				users[findUserByID(users, user.userID)].bookings[bookingID].startPeriod = encodePeriods(input);
				user.bookings[bookingID].startPeriod = encodePeriods(input);
				if (encodePeriods(input) < user.bookings[bookingID].startPeriod) {
					for (int i = encodePeriods(input); i < user.bookings[bookingID].startPeriod; i++) {
						rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[user.bookings[bookingID].startDay][i] = false;
					}
				}
				else {
					for (int i = user.bookings[bookingID].startPeriod; i < encodePeriods(input); i++) {
						rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[user.bookings[bookingID].startDay][i] = true;
					}
				}
				break;
			case 3:
				cout << "Enter The New End Day\n";
				cin >> input;
				if (encodeDays(input) < user.bookings[bookingID].startDay) {
					cout << "Can Not Edit The End Day\n";
				}
				else {
					users[findUserByID(users, user.userID)].bookings[bookingID].endDay = encodeDays(input);
					user.bookings[bookingID].endDay = encodeDays(input);
					for (int day = user.bookings[bookingID].startDay; day <= user.bookings[bookingID].endDay; day++) {
						for (int period = 0; period < PERIODS; period++) {
							if ((user.bookings[bookingID].startDay == day && user.bookings[bookingID].startPeriod <= period) || (user.bookings[bookingID].startDay < day && user.bookings[bookingID].endDay > day) || (user.bookings[bookingID].endDay == day && user.bookings[bookingID].endPeriod >= period)) {
								rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[day][period] = true;
							}
							else
								continue;
						}
					}
					for (int day = user.bookings[bookingID].startDay; day <= encodeDays(input); day++) {
						for (int period = 0; period < PERIODS; period++) {
							if ((user.bookings[bookingID].startDay == day && user.bookings[bookingID].startPeriod <= period) || (user.bookings[bookingID].startDay < day && user.bookings[bookingID].endDay > day) || (user.bookings[bookingID].endDay == day && user.bookings[bookingID].endPeriod >= period)) {
								rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[day][period] = false;
							}
							else
								continue;
						}
					}
				}
				break;;
			case 4:
				cout << "Enter The New End Period\n";
				cin >> input;
				users[findUserByID(users, user.userID)].bookings[bookingID].endPeriod = encodePeriods(input);
				user.bookings[bookingID].endPeriod = encodePeriods(input);
				if (encodePeriods(input) < user.bookings[bookingID].endPeriod) {
					for (int i = encodePeriods(input) + 1; i <= user.bookings[bookingID].startPeriod; i++) {
						rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[user.bookings[bookingID].endDay][i] = true;
					}
				}
				else {
					for (int i = user.bookings[bookingID].endPeriod + 1; i <= encodePeriods(input); i++) {
						rooms[findRoomById(rooms, user.bookings[bookingID].roomID)].availability[user.bookings[bookingID].endDay][i] = false;
					}
				}
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