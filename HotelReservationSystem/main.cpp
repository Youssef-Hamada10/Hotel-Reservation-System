#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

const int DAYS = 7;
const int PERIODS = 3;
const int bookings_count = 100;

struct Room{
	int roomID;
	string roomType;
	string location;
	bool availability[DAYS][PERIODS];
	float price;
};

struct Booking{
	int bookingID = 0;
	int userID = 0;
	int roomID = 0;
	int startDay = 0;
	int startperiode = 0;
	int endDay = 0;
	int endperiode = 0;
};

struct User{
	int userID;
	string userType;
	string username;
	string password;
	Booking bookings[bookings_count];
};

//USER registeration
bool Register(vector<User>& users);
void login();
string findusertype(User user, vector<User> users);
bool findID(int IDnum, vector<User>users);
bool findsign(string username, string password, vector<User>users);
bool findsign(string username, string password, vector<User>users);
bool findlogin(string username, string password, vector<User>users);
int menu();
int anotheruserindex;
int thisuserindex;

// ADMIN functions
void add_room(vector<Room>& rooms);
void readRooms(vector<Room>& rooms);
void writeRooms(vector<Room>rooms);
void readUsers(vector<User>& users);
void writeUsers(vector<User> users);
void adminMenu(vector<Room>& rooms, vector<User>users);
void manageRooms(vector<Room>& rooms, vector<User>users);
void manageRoomsMenu(vector<Room>rooms);
void editRoomMenu();
void editRoom(Room& room, vector<Room>& rooms, vector<User>users);
void check_booking(int roomID, vector<Room>& rooms, vector<User>users);
bool findRoom(int roomID, vector<Room>rooms);
bool findBooking(int roomID, vector<User>users);
string replace(string str, char replaceChar, char replaceWith);
Room getRoomById(int roomID, vector<Room>rooms);
bool findRoomByType(string roomType, vector<Room>rooms);
int getBookingIndex(int bookingID, User user);
bool findBookingId(int bookingID, User user);

// BOOKING functions
void modify(int bookingIndex, vector<User>& users);
void usermenu();
void book_room();
void view_booking(vector<User>& users, vector<Room>rooms);
int RoomsMenu(vector<Room>rooms, string type, int startDay, int startPeriod, int endDay, int endPeriod);
bool findbookID(int IDnum, vector<User>users);
string getType(vector<Room>rooms);
string getDay(int daynum);
string getTime(int Time);
bool checkAvailability(Room room, int startDay, int startPeriod, int endDay, int endPeriod);

// Global vectors
vector<Room>rooms;
vector<User>users;

int main() {
	int choice;
	char answer;

	readRooms(rooms);
	readUsers(users);

	do {
		choice = menu();
		switch (choice)
		{
		case 1:
		{
			Register(users);
			break;
		}
		case 2:
		{
			login();
			break;
		}
		default:
		{
			cout << "Wrong choice, please re-enter the choice \n";
			answer = 'y';
			continue;
		}
		}
		cout << "Do you want to do another operation (Y/N) \n";
		cin >> answer;
	} while (answer == 'Y' || answer == 'y');
	cout << "\n\t\t\t   Thank you for registering with us  " << endl;
	writeUsers(users);
	writeRooms(rooms);

}

//user rigistration

bool Register(vector<User>& users)
{
	User temp;
	cout << "Enter your username \n";
	cin >> temp.username;
	cout << "Enter your password \n";
	cin >> temp.password;
	if (findsign(temp.username, temp.password, users))
	{
		cout << "\n\t\t username or password are taken \n";
		return false;
	}
	else
	{
		cout << "\n\t\t The registration is succeeded \n";
	}
	temp.userID = (rand() % 101);
	while (findID(temp.userID, users))
	{
		temp.userID = (rand() % 101);
	}

	temp.userType = "user";

	users.push_back(temp);
	return true;

}
void login()
{
	User temp;
	cout << "Enter your username \n";
	cin >> temp.username;
	cout << "Enter your password \n";
	cin >> temp.password;

	if (findlogin(temp.username, temp.password, users) == true && findusertype(temp, users) == "user")
	{
		cout << "\n\t\t Login is succeeded \n";
		cout << "\n\t\t WELCOME Mr." << temp.username << endl;
		usermenu();

	}
	else if (findlogin(temp.username, temp.password, users) == true && findusertype(temp, users) == "admin")
	{
		cout << "\n\t\t Login is succeeded \n";
		cout << "\n\t\t    WELCOME ADMIN   \n";
		adminMenu(rooms, users);

	}
	else
	{
		cout << " Username or Password is incorrect \n";
	}
}
bool findlogin(string username, string password, vector<User>users)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (username == users[i].username && password == users[i].password)
		{
			thisuserindex = i;
			return true;
		}
	}
	return false;
}
bool findsign(string username, string password, vector<User>users)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (username == users[i].username && password == users[i].password)
		{
			anotheruserindex = i;
			return true;
		}
	}
	return false;
}
bool findID(int IDnum, vector<User>users)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (IDnum == users[i].userID || IDnum == 1)
			return true;
	}
	return false;
}
int menu()
{
	int choice;
	cout << "1: Register\n2: Login\nYour choice: ";
	cin >> choice;
	return choice;
}
string findusertype(User user, vector<User>users)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (user.password == users[i].password && user.username == users[i].username) {
			if (users[i].userType == "admin") {
				return "admin";
			}
			else if (users[i].userType == "user") {
				return "user";
			}
		}
	}
}

//admin functions

void readRooms(vector<Room>& rooms) {
	Room room;
	fstream in("rooms.txt", ios::in);
	if (!in) {
		cout << "file not found" << endl;
		in.close();
		return;
	}
	for (int i = 0; !in.eof(); i++) {
		in >> room.roomID >> room.roomType >> room.location >> room.price;
		room.location = replace(room.location, '_', ' ');
		for (int j = 0; j < DAYS; j++) {
			for (int k = 0; k < PERIODS; k++) {
				in >> room.availability[j][k];
			}
		}
		if (room.roomID > 0) {
			rooms.push_back(room);
		}
	}
	in.close();
}
void writeRooms(vector<Room>rooms) {
	fstream out("rooms.txt", ios::out);
	if (!out)
	{
		cout << "file not found" << endl;
		out.close();
		return;
	}
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomID > 0) {
			out << endl;
			out << rooms[i].roomID << " " << rooms[i].roomType << " " << replace(rooms[i].location, ' ', '_') << " " << rooms[i].price;
			for (int j = 0; j < DAYS; j++) {
				for (int k = 0; k < PERIODS; k++) {
					out << " " << rooms[i].availability[j][k];
				}
			}
		}
	}
	out.close();
}
void readUsers(vector<User>& users) {
	User user;
	fstream in("users.txt", ios::in);
	if (!in)
	{
		cout << "file not found" << endl;
		in.close();
		return;
	}
	for (int i = 0; !in.eof(); i++)
	{
		in >> user.userID >> user.username >> user.userType >> user.password;
		user.username = replace(user.username, '_', ' ');
		for (int j = 0; j < bookings_count; j++) {
			in >> user.bookings[j].bookingID >> user.bookings[j].roomID >> user.bookings[j].userID >> user.bookings[j].startDay >> user.bookings[j].startperiode >> user.bookings[j].endDay >> user.bookings[j].endperiode;
		}
		if (user.userID > 0)
		{
			users.push_back(user);
		}
	}
	in.close();
}
void writeUsers(vector<User>users) {
	User user;
	fstream out("users.txt", ios::out);
	if (!out) {
		cout << "file not found" << endl;
		out.close();
		return;
	}
	for (int i = 0; i < users.size(); i++) {
		out << endl;
		out << users[i].userID << " " << replace(users[i].username, ' ', '_') << " " << users[i].userType << " " << users[i].password;
		for (int j = 0; j < bookings_count; j++) {
			out << " " << users[i].bookings[j].bookingID << " " << users[i].bookings[j].roomID << " " << users[i].bookings[j].userID << " " << users[i].bookings[j].startDay << " " << users[i].bookings[j].startperiode << " " << users[i].bookings[j].endDay << " " << users[i].bookings[j].endperiode;
		}
	}
	out.close();
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
void adminMenu(vector<Room>& rooms, vector<User>users) {
	int choice;   //local variable for the choice
	while (true)
	{
		cout << "Press: " << endl << endl;
		cout << "( 1 ) To add room :\n";
		cout << "( 2 ) To edit room :\n";
		cout << "( 3 ) To exit :\n";
		cin >> choice;
		if (choice == 1)
		{
			add_room(rooms);
		}
		else if (choice == 2)
		{
			manageRooms(rooms, users);
		}
		else if (choice == 3)
		{
			break;
		}
		else
		{
			cout << "invalid choice! \n";
		}
	}
}
void manageRooms(vector<Room>& rooms, vector<User>users) {
	int roomID;
	int choiceNum;
	bool exit = false;
	do
	{
		manageRoomsMenu(rooms);
		if (rooms.size() > 0) {
			cin >> choiceNum;
			if (choiceNum == 2) {
				exit = true;
			}
			else if (choiceNum == 1) {
				cout << endl << "Enter ID of the room you want to edit: " << endl;
				cin >> roomID;
				while (true) {
					if (findRoom(roomID, rooms) && roomID != 0) {
						break;
					}
					cout << "Please enter a valid room ID" << endl;
					cout << "Enter the room ID to you want to edit: " << endl;
					cin >> roomID;
				}
				for (int i = 0; i < rooms.size(); i++) {
					if (rooms[i].roomID == roomID) {
						editRoomMenu();
						editRoom(rooms[i], rooms, users);
					}
				}
			}
			else {
				cout << "Enter a valid choice number" << endl;
			}
		}
		else {
			exit = true;
		}
	} while (!exit);
}
bool findRoom(int roomID, vector<Room>rooms) {
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomID == roomID) {
			return true;
		}
	}
	return false;
}
void manageRoomsMenu(vector<Room>rooms) {
	if (rooms.size() > 0) {
		cout << endl << "\t\t\t\t\t\tManage Rooms" << endl << endl;
		for (int i = 0; i < rooms.size(); i++)
		{
			cout << "------------------------------------------------" << endl;
			cout << "RoomID | type     |    Loctaion   | Price      |" << endl;
			cout << "------------------------------------------------" << endl;
			if (rooms[i].roomID > 0)
			{
				cout << rooms[i].roomID << "    |  " << rooms[i].roomType << "  |  " << rooms[i].location << "   |  " << rooms[i].price << "      |" << endl;
				cout << "------------------------------------------------" << endl;
				cout << "|                   Availability               |" << endl;
				cout << "------------------------------------------------" << endl;
				cout << "( Period )    Day  1   |    Day  2   |    Day  3   |    Day  4   |    Day  5   |    Day  6   |    Day  7   | ";
				for (int j = 0; j < PERIODS; j++)
				{
					cout << endl;
					if (j == 0) {
						cout << "Morning:  ";
					}
					else if (j == 1) {
						cout << "Afternoon:";
					}
					else if (j == 2) {
						cout << "Evening:  ";
					}
					for (int k = 0; k < DAYS; k++) {
						if (rooms[i].availability[k][j]) {
							cout << "     AVBL    |";
						}
						else {
							cout << "     UNAVBL  |";
						}
					}
				}
			}
			cout << "\n\n\t\t\t\t\t         **************************************************          \n\n";
		}
		cout << endl << endl << "Press: " << endl << endl;
		cout << "( 1 ) Select room to edit or show bookings: " << endl;
		cout << "( 2 ) Exit" << endl;
		cout << endl << "Enter Choice Number: " << endl;
	}
	else {
		cout << "No Rooms To Display" << endl;
	}
}
void editRoomMenu() {
	cout << endl << "Press:  " << endl << endl;
	cout << "( 1 )  Edit Room Type" << endl;
	cout << "( 2 )  Edit Room Price" << endl;
	cout << "( 3 )  Edit Room Location" << endl;
	cout << "( 4 )  Edit Room Availability" << endl;
	cout << "( 5 )  Edit Room ID" << endl;
	cout << "( 6 )  Check Bookings" << endl;
	cout << "( 7 )  Remove Room" << endl;
	cout << "( 8 )  Go Back To Manage Rooms Menu" << endl << endl;
}
void editRoom(Room& room, vector<Room>& rooms, vector<User>users) {
	int choiceNum;
	do
	{
		cout << "Enter Choice Number: " << endl;
		cin >> choiceNum;
		switch (choiceNum)
		{
		case 1:
			cout << "Enter Room Type: " << endl;
			cin >> room.roomType;
			break;
		case 2:
			cout << "Enter Room Price: " << endl;
			cin >> room.price;
			break;
		case 3:
			cout << "Enter Room Location: " << endl;
			getline(cin.ignore(1, '\n'), room.location);
			break;
		case 4:
			int choice;
			cout << "Press: " << endl << endl;
			cout << "( 1 ) To make room available" << endl;
			cout << "( 2 ) To make room unavailable" << endl << endl;
			cout << "Enter choice number: " << endl;
			while (true)
			{
				cin >> choice;
				if (choice == 1) {
					for (int i = 0; i < DAYS; i++)
					{
						for (int j = 0; j < PERIODS; j++)
						{
							room.availability[i][j] = 1;
						}
					}
					cout << "Room availability changed successfully" << endl;
					break;
				}
				else if (choice == 2) {
					for (int i = 0; i < DAYS; i++)
					{
						for (int j = 0; j < PERIODS; j++)
						{
							room.availability[i][j] = 0;
						}
					}
					cout << "Room availability changed successfully" << endl;
					break;
				}
				else {
					cout << "Enter a valid choice !" << endl;
				}
			}
			break;
		case 5:
			int roomID;
			while (true) {
				cout << "Enter a valid new ID: " << endl;
				cin >> roomID;
				if ((findRoom(roomID, rooms) && roomID != room.roomID) || roomID <= 0) {
					cout << "Invalid Room ID" << endl;
				}
				else {
					room.roomID = roomID;
					break;
				}
			}
			break;
		case 6:
			check_booking(room.roomID, rooms, users);
			break;
		case 7:
			for (int i = 0; i < rooms.size(); i++) {
				if (rooms[i].roomID == room.roomID) {
					rooms.erase(rooms.begin() + i, rooms.begin() + i + 1);
					break;
				}
			}
			break;
		case 8:
			break;
		default:
			cout << "Enter a valid choice number" << endl;
			break;
		}
	} while (choiceNum != 7 && choiceNum != 8);
}
void add_room(vector<Room>& rooms)    //function to add rooms
{
	Room temp;  //temporerly variable to assign the rooms
	int choice;

	while (true) {
		cout << "enter room ID: \n";
		cin >> temp.roomID;
		if (!findRoom(temp.roomID, rooms)) {
			break;
		}
		cout << "A room with that ID already exists! " << endl;
	}

	cout << "enter room type: \n";
	cin >> temp.roomType;

	cout << "enter room price: \n";
	cin >> temp.price;

	cout << "enter room location: " << endl;
	getline(cin.ignore(1, '\n'), temp.location);

	cout << "assign room availability\n\n";
	while (true)
	{
		cout << "press: \n";
		cout << "( 1 )  For available\n";
		cout << "( 2 )  For unavailable\n";
		cin >> choice;
		if (choice == 1)
		{
			for (int i = 0; i < DAYS; i++)
			{
				for (int j = 0; j < PERIODS; j++)
				{
					temp.availability[i][j] = 1;
				}
			}
			break;
		}
		else if (choice == 2)
		{
			for (int i = 0; i < DAYS; i++)
			{
				for (int j = 0; j < PERIODS; j++)
				{
					temp.availability[i][j] = 0;
				}
			}
			break;
		}
		else
		{
			cout << "incorrect choice !\n";
		}
	}

	rooms.push_back(temp);
	cout << "The room is added successfully.\n";
}
void check_booking(int roomID, vector<Room>& rooms, vector<User>users)  //function to check booking using id 
{
	int booking_count = 0;
	if (rooms.empty())
	{
		cout << "There is no rooms to show.\n";
		return;              //to terminate the function
	}
	if (findBooking(roomID, users))
	{
		cout << "\t\t\tBookings for room " << roomID << endl;
		cout << "--------------------------------------------------------------------------" << endl;
		cout << "Room ID | Booking ID | User ID | Start Day | End Day |" << endl;
		cout << "--------------------------------------------------------------------------" << endl;
		for (int i = 0; i < users.size(); i++) {
			for (int j = 0; j < bookings_count; j++)
			{
				if (users[i].bookings[j].roomID == roomID && users[i].bookings[j].bookingID != 0) {
					cout << users[i].bookings[j].roomID << "\t|      " << users[i].bookings[j].bookingID << "    |\t  " << users[i].bookings[j].userID << "   |      " << users[i].bookings[j].startDay << "    |    " << users[i].bookings[j].endDay << "    |" << endl;
					booking_count++;
				}
			}
		}
	}
	if (booking_count == 0)
	{
		cout << "This room has no bookings! " << endl;
	}
	else
	{
		cout << "This room has " << booking_count << " bookings" << endl;
	}
}
bool findBooking(int roomID, vector<User>users) {
	for (int i = 0; i < users.size(); i++) {
		for (int j = 0; j < bookings_count; j++) {
			if (users[i].bookings[j].roomID == roomID && roomID != 0)
			{
				return true;
			}
		}
	}
	return false;
}

//booking function
string getDay(int daynum)
{
	if (daynum == 1)
	{
		return "Sunday ";
	}
	else if (daynum == 2) {
		return "Monday";
	}
	else if (daynum == 3) {
		return "Tuesday";
	}
	else if (daynum == 4) {
		return "Wednesday";
	}
	else if (daynum == 5) {
		return "Thursday";
	}
	else if (daynum == 6) {
		return "Friday";
	}
	else if (daynum == 7) {
		return "Saturday";
	}
}
string getTime(int Time)
{
	if (Time == 1) {
		return "Morning";
	}
	else if (Time == 2) {
		return "Afternoon";
	}
	else if (Time == 3) {
		return "Evening";
	}

}
/*string getType(vector<Room>rooms)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i].roomType == "Regular")
		{
			return "Regular";
		}
		else if (rooms[i].roomType == "Vip") {
			return "Vip";
		}
		else if (rooms[i].roomType == "Suite") {
			return "Suite";
		}
	}
}*/
void book_room() {
	cout << " notice : the hotel work for 7 days only so you can not stay more thn 7 days \t\t sorry\n\n";
	cout << " notice : You can only book rooms on days with valid numbers. You cannot book \t\t sorry\n\n";
	cout << " notice : If you book less than a day, you will be charged a full day \t\t sorry\n\n";

	bool check = true;
	bool booking_success = false;
	Room temproom;
	Booking tempbook;
	int cycle = thisuserindex;

	while (true)
	{
		cout << "enter the date of your arrival\n" << endl << " enter the days:\t\t  \n1-Sunday\n2-Monday\n3-Tuesday\n4-Wednesday\n5-Thursday\n6-Friday\n7-Saturday \n";
		cin >> tempbook.startDay;
		while (tempbook.startDay < 1 || tempbook.startDay > 7)
		{
			cout << " you enter a wrong day \n\n please enter a correct day \n\n";
			cin >> tempbook.startDay;
		}
		cout << "\nEnter the time period you will start  (1-3) \t (1 for morning, 2 for afternoon, 3 for evening) : ";
		cin >> tempbook.startperiode;

		while (tempbook.startperiode < 1 || tempbook.startperiode > 3)
		{
			cout << " you enter a wrong Time Period \n\n please enter a correct Time Period \n\n";
			cin >> tempbook.startperiode;
		}

		cout << "enter the date of the end for your booking \n" << endl;
		cout << " enter the day\t\t   \n1-Sunday\n2-Monday\n3-Tuesday\n4-Wednesday\n5-Thursday\n6-Friday\n7-Saturday \n";
		cin >> tempbook.endDay;

		while (tempbook.endDay < 1 || tempbook.endDay > 7 || tempbook.startDay > tempbook.endDay)
		{
			cout << " you enter a wrong day \n\n please enter a correct day \n\n";
			cin >> tempbook.endDay;
		}
		cout << "\nEnter the time period you will end  (1-3) \t (1 for morning, 2 for afternoon, 3 for evening) : ";
		cin >> tempbook.endperiode;

		while (tempbook.endperiode < 1 || tempbook.endperiode > 3)
		{
			cout << " you enter a wrong Periods \n\n please enter a correct Periods \n\n";
			cin >> tempbook.endperiode;
		}
		if (tempbook.startDay > tempbook.endDay || tempbook.startDay == tempbook.endDay && tempbook.startperiode > tempbook.endperiode) {
			cout << "Date of arrival can not be after the day of leave." << endl;
		}
		else {
			break;
		}
	}

	while (check)
	{
		cout << " Write the room type you would like\t 1- Regular \t2- VIP \t3- Suite : " << endl;
		cin >> temproom.roomType;
		if (temproom.roomType == "regular" || temproom.roomType == "Regular" || temproom.roomType == "REGULAR")
		{
			cout << " The reservation details you entered : \n";
			if (findRoomByType("regular", rooms))
			{
				if (RoomsMenu(rooms, "regular", tempbook.startDay, tempbook.startperiode, tempbook.endDay, tempbook.endperiode) > 0)
					check = false;
			}
			else {
				cout << "No rooms with your preferred choices are available" << endl;
			}
		}
		else if (temproom.roomType == "vip" || temproom.roomType == "Vip" || temproom.roomType == "VIP")
		{
			cout << " The reservation details you entered : \n";
			if (findRoomByType("vip", rooms)) {
				if (RoomsMenu(rooms, "vip", tempbook.startDay, tempbook.startperiode, tempbook.endDay, tempbook.endperiode) > 0)
					check = false;
			}
			else {
				cout << "No rooms with your preferred choices are available" << endl;
			}
		}
		else if (temproom.roomType == "suite" || temproom.roomType == "Suite" || temproom.roomType == "SUITE")
		{
			cout << " The reservation details you entered : \n";
			if (findRoomByType("suite", rooms))
			{
				if (RoomsMenu(rooms, "suite", tempbook.startDay, tempbook.startperiode, tempbook.endDay, tempbook.endperiode) > 0)
					check = false;
			}
			else {
				cout << "No rooms with your preferred choices are available" << endl;
			}
		}
		else
		{
			cout << " you enter a wrong type please WRITE the correct type : ";
		}
	}

	cout << "enter the room ID you want to book" << endl;
	cin >> temproom.roomID;

	tempbook.bookingID = (rand() % 101) + 100;
	while (findbookID(tempbook.bookingID, users))
	{
		tempbook.bookingID = (rand() % 101) + 100;
	}

	for (int i = 0; i < bookings_count; i++)
	{
		if (users[thisuserindex].bookings[i].bookingID == 0)
		{
			users[thisuserindex].bookings[i].bookingID = tempbook.bookingID;
			users[thisuserindex].bookings[i].roomID = temproom.roomID;
			users[thisuserindex].bookings[i].userID = users[thisuserindex].userID;
			users[thisuserindex].bookings[i].startDay = tempbook.startDay;
			users[thisuserindex].bookings[i].startperiode = tempbook.startperiode;
			users[thisuserindex].bookings[i].endDay = tempbook.endDay;
			users[thisuserindex].bookings[i].endperiode = tempbook.endperiode;
			booking_success = true;
			break;
		}
	}
	if (booking_success)
	{
		cout << "The room is booked successfully.\n";
	}
	else {
		cout << "Booking failed! You have reached the maximunm bookings limit.\n";
	}
}
void view_booking(vector<User>& users, vector<Room>rooms)
{
	int bookings = 0;
	//show the user the booking details
	for (int i = 0; i < bookings_count; i++) {
		if (users[thisuserindex].bookings[i].bookingID > 0) {
			if (bookings == 0) {
				cout << "<<<----------------------------------->>>\n";
				cout << " your booking details is : \n";
			}
			cout << "<<<---------------------->>>\n\n";
			cout << "booking ID :" << users[thisuserindex].bookings[i].bookingID;
			cout << "       name: " << users[thisuserindex].username << "    price : " << rooms[thisuserindex].price << endl << endl;
			cout << "<<<------------------------------------------------------------------------------------------------>>>\n\n";
			cout << " your Day of booking start at : ";
			cout << getDay(users[thisuserindex].bookings[i].startDay) << "----->>> " << getTime(users[thisuserindex].bookings[i].startperiode) << endl << endl;
			cout << "<<<------------------------------------------------------------------------------------------------>>>\n\n";
			cout << "\n" << "And booking  end at : ";
			cout << getDay(users[thisuserindex].bookings[i].endDay) << "----->>> " << getTime(users[thisuserindex].bookings[i].endperiode) << endl << endl;
			cout << "<<<------------------------------------------------------------------------------------------------>>>\n\n";
			cout << "your room ID is : " << users[thisuserindex].bookings[i].roomID << "\n\n" << "Room type is : " << getRoomById(users[thisuserindex].bookings[i].roomID, rooms).roomType << endl;
			cout << "\n<<<------------------------------------------------------------------------------------------------>>>\n\n";
			bookings++;
		}
	}
	if (bookings == 0)
	{
		cout << "You have no bookings yet !" << endl;
	}
	else {
		cout << "You have " << bookings << " bookings" << endl;
		int choice;
		cout << "Press: " << endl << endl;
		cout << "( 1 ) To modify booking." << endl;
		cout << "( 2 ) Go back" << endl;
		while (true)
		{
			cin >> choice;
			if (choice == 1) {
				int bookingId;
				while (true)
				{
					cout << "Enter booking ID to modify: " << endl;
					cin >> bookingId;
					if (findBookingId(bookingId, users[thisuserindex])) {
						modify(getBookingIndex(bookingId, users[thisuserindex]), users);
						break;
					}
					else {
						cout << "Enter a valid booking ID" << endl;
					}
				}
				break;
			}
			else if (choice == 2) {
				break;
			}
			else {
				cout << "Enter a valid choice" << endl;
			}
		}
	}

}
Room getRoomById(int roomID, vector<Room>rooms)
{
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomID == roomID) {
			return rooms[i];
		}
	}
}
int getBookingIndex(int bookingID, User user)
{
	for (int i = 0; i < bookings_count; i++)
	{
		if (user.bookings[i].bookingID == bookingID)
		{
			return i;
		}
	}
}
void modify(int bookingIndex, vector<User>& users)
{
	int choice;
	Booking tempbook;
	cout << "1:Modify endDay\n2:Book another room\n3:Remove Booking\n";
	while (true)
	{
		cout << "Please enter the number of the thing you want to modify :";
		cin >> choice;
		if (choice == 1)
		{
			cout << "enter the date of the end for your booking \n" << endl;
			cout << " enter the day\t\t   \n1-Sunday\n2-Monday\n3-Tuesday\n4-Wednesday\n5-Thursday\n6-Friday\n7-Saturday \n";
			cin >> tempbook.endDay;

			while (tempbook.endDay < 1 || tempbook.endDay > 7 || tempbook.startDay > tempbook.endDay)
			{
				cout << " you enter a wrong day \n\n please enter a correct day \n\n";
				cin >> tempbook.endDay;
			}
			cout << "\nEnter the time period you will end  (1-3) \t (1 for morning, 2 for afternoon, 3 for evening) : ";
			cin >> tempbook.endperiode;

			while (tempbook.endperiode < 1 || tempbook.endperiode > 3 || tempbook.startperiode >= tempbook.endperiode)
			{
				cout << " you enter a wrong Periods \n\n please enter a correct Periods \n\n";
				cin >> tempbook.endperiode;
			}
			users[thisuserindex].bookings[bookingIndex].endDay = tempbook.endDay;
			users[thisuserindex].bookings[bookingIndex].endperiode = tempbook.endperiode;
			break;
		}
		else if (choice == 2)
		{
			book_room();
			break;
		}
		else if (choice == 3) {
			users[thisuserindex].bookings[bookingIndex].bookingID = 0;
			users[thisuserindex].bookings[bookingIndex].roomID = 0;
			users[thisuserindex].bookings[bookingIndex].userID = 0;
			users[thisuserindex].bookings[bookingIndex].startDay = 0;
			users[thisuserindex].bookings[bookingIndex].startperiode = 0;
			users[thisuserindex].bookings[bookingIndex].endDay = 0;
			users[thisuserindex].bookings[bookingIndex].endperiode = 0;
			break;
		}
		else {
			cout << "Invalid Choice !" << endl;
		}
	}
}
int RoomsMenu(vector<Room>rooms, string type, int startDay, int startPeriod, int endDay, int endPeriod)
{
	int rooms_count = 0;
	if (rooms.size() > 0)
	{
		for (int i = 0; i < rooms.size(); i++)
		{
			if (rooms[i].roomID > 0 && checkAvailability(rooms[i], startDay, startPeriod, endDay, endPeriod) && rooms[i].roomType == type)
			{
				rooms_count++;
				cout << "------------------------------------------------" << endl;
				cout << "RoomID | type     |    Loctaion   | Price      |" << endl;
				cout << "------------------------------------------------" << endl;
				cout << rooms[i].roomID << "    |  " << rooms[i].roomType << "  |  " << rooms[i].location << "   |  " << rooms[i].price << "      |" << endl;
				cout << "------------------------------------------------" << endl;
				cout << "|                   Availability               |" << endl;
				cout << "------------------------------------------------" << endl;
				cout << "( Period )    Day  1   |    Day  2   |    Day  3   |    Day  4   |    Day  5   |    Day  6   |    Day  7   | ";
				for (int j = 0; j < PERIODS; j++)
				{
					cout << endl;
					if (j == 0) {
						cout << "Morning:  ";
					}
					else if (j == 1) {
						cout << "Afternoon:";
					}
					else if (j == 2) {
						cout << "Evening:  ";
					}
					for (int k = 0; k < DAYS; k++) {
						if (rooms[i].availability[k][j]) {
							cout << "     AVBL    |";
						}
						else {
							cout << "     UNAVBL  |";
						}
					}
				}
				cout << "\n\n\t\t\t\t\t         **************************************************          \n\n";
			}
		}
	}
	if (rooms_count == 0)
	{
		cout << "\t\t\t\t\t\tNo Rooms To Available Display with this type " << endl;
	}
	return rooms_count;
}
void usermenu()
{
	char answer;
	cout << "\n\n\t\t\t\t\t Welcome to user page \n";
	cout << "what the action do you want to do\n";
	int choice;   //local variable for the choice
	while (true)
	{
		cout << "Press: " << endl << endl;
		cout << "( 1 ) To book a room :\n";
		cout << "( 2 ) To View your booked room :\n";
		cout << "( 3 ) To exit :\n";
		cin >> choice;
		if (choice == 1)
		{
			book_room();
		}
		else if (choice == 2)
		{
			view_booking(users, rooms);
		}
		else if (choice == 3)
		{
			cout << "thank you for dealing with us \n";
			break;
		}
		else
		{
			cout << "invalid choice! \n";
		}
	}

}
bool findbookID(int IDnum, vector<User>users)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (IDnum == users[i].bookings[i].bookingID)
			return true;
	}
	return false;
}
bool findRoomByType(string roomType, vector<Room>rooms) {
	for (int i = 0; i < rooms.size(); i++) {
		if (rooms[i].roomType == roomType) {
			return true;
		}
	}
	return false;
}
bool findBookingId(int bookingID, User user) {
	for (int i = 0; i < bookings_count; i++) {
		if (user.bookings[i].bookingID == bookingID) {
			return true;
		}
	}
	return false;
}
bool checkAvailability(Room room, int startDay, int startPeriod, int endDay, int endPeriod) {
	for (int i = startDay - 1; i < endDay; i++) {
		if (i == endDay - 1 && i == startDay - 1) {
			cout << "aaaa" << endl;
			for (int j = startPeriod - 1; j < endPeriod; j++) {
				if (!room.availability[i][j]) {
					return false;
				}
			}
		}
		else if (i == endDay - 1) {
			for (int j = 0; j < endPeriod; j++) {
				if (!room.availability[i][j]) {
					return false;
				}
			}
		}
		else if (i == startDay - 1) {
			for (int j = startPeriod - 1; j < 3; j++) {
				if (!room.availability[i][j]) {
					return false;
				}
			}
		}
		else {
			for (int j = 0; j < 3; j++) {
				if (!room.availability[i][j]) {
					return false;
				}
			}
		}
	}
	return true;
}
