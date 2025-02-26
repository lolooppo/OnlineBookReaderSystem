#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace std;

int ReadInt (const int& low, const int& high) {
    int value;
    cin >>  value;

    if(value >= low && value <= high)
        return value;

    cerr    <<  "Error, Please try again...\n";
    return ReadInt(low, high);
}

int ShowReadMenu (const vector<string>& choices) {
    cerr    <<  "\nMenu...\n";
    for(int choice = 0; choice < (int)choices.size(); choice++){
        cerr    <<  "\t"  <<  choice + 1  <<  ")" <<  choices[choice] <<  endl;
    }
    return ReadInt(1, (int)choices.size());
}

string GetCurrentTimeDate () {
    auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
}


///////////////////Book///////////////////
class Book {
private:
    string title{""};
    string author_name{""};
    string ISBN{""};
    vector<string> pages;

public:

    void Read () {
        string str;

        cerr    <<  "Enter Title(no spaces):   ";
        cin     >>  str;
        SetTitle(str);

        cerr    <<  "Enter Author Name(no spaces): ";
        cin     >>  str;
        SetAuthorName(str);

        cerr    <<  "Enter ISBN:    ";
        cin     >>  str;
        SetISBN(str);

        int pages_count;
        cerr    <<  "Enter pages count: ";
        cin     >>  pages_count;
        for(int i = 0; i < pages_count; i++){
            string page;
            cerr    <<  "\tEnter page #"  <<  (i+1) <<":    ";
            cin     >>  page;
            pages.push_back(page);
        }
    }


    void SetTitle (const string& title) {
        this->title = title;
    }

    const string& GetTitle() const {
        return title;
    }

    void SetAuthorName (const string& author_name) {
        this->author_name = author_name;
    }

    const string& GetAuthorName () const {
        return author_name;
    }

    void SetISBN (const string& ISBN) {
        this->ISBN = ISBN;
    }

    const string& GetISBN () const {
        return ISBN;
    }

    const vector<string>& GetPages () const {
        return pages;
    }

    int GetPagesCount () const {
        return (int)pages.size();
    }

    void SetPages (const vector<string>& pages) {
        this->pages = pages;
    }

    string ToString (){
        ostringstream oss;

        oss <<  "\tName:  "   <<  GetTitle()  <<  "\n";
        oss <<  "\tAuthor Name:   "   <<  GetAuthorName() <<  "\n";
        oss <<  "\tPages Count:   "   <<  GetPagesCount();

        return oss.str();
    }
};



////////////////////Book Reading Session//////////////////////////////////
class BookReadingSession {
private:
    Book* book;
    int current_page{1};
    string last_time{""};

public:
    BookReadingSession () :
        BookReadingSession(nullptr){
    }

    BookReadingSession(Book* book) :
        book(book), last_time(GetCurrentTimeDate()){
    }

    string GetCurrentPageContent () const {
        return book->GetPages()[current_page-1];
    }

    const int& GetCurrentPage () const {
        return current_page;
    }

    void NextPage () {
        if(current_page == book->GetPagesCount())
            return;
        current_page++;
    }

    void PrevPage () {
        if(current_page == 1)
            return;
        current_page--;
    }

    string PageIdx () {
        ostringstream oss;
        oss <<  GetCurrentPage()    <<  "/" <<  book->GetPagesCount();
        return oss.str();
    }

    void ResetLastTime () {
        last_time = GetCurrentTimeDate();
    }

    const string& GetLastTime () const {
        return last_time;
    }

    const Book* GetBook () const {
        return this->book;
    }

    string ToString () {
        ostringstream oss;
        oss << "Book Name:  " <<  book->GetTitle()    <<  ", current page:    " <<  PageIdx()    <<  ", Last Time:  "   <<  GetLastTime();
        return oss.str();
    }
};




//////////////////////User///////////////////
class User {
private:
    string name{""};
    string user_name{""};
    string password{""};
    string email{""};
    bool is_admin{false};
    vector<BookReadingSession*> reading_sessions;

public:


    ~User () {
        for(auto& session : reading_sessions){
            delete session;
        }
        reading_sessions.clear();
    }

    void Read (const string& user_name) {
        SetUserName(user_name);

        string str;

        cerr    <<  "Enter Name(no spaces):    ";
        cin     >>  str;
        SetName(str);

        cerr    <<  "Enter Password(no spaces):    ";
        cin     >>  str;
        SetPassword(str);

        cerr    <<  "Enter Email(no spaces):    ";
        cin     >>  str;
        SetEmail(str);
    }

    void SetUserName (const string& user_name) {
        this->user_name = user_name;
    }

    const string& GetUserName () const {
        return user_name;
    }

    void SetName (const string& name) {
        this->name = name;
    }

    const string& GetName () const{
        return name;
    }

    void SetEmail (const string& email) {
        this->email = email;
    }

    const string& GetEmail () const {
        return email;
    }

    void SetPassword (const string& password) {
        this->password = password;
    }

    const string& GetPassword () const {
        return password;
    }

    string ToString () const {
        ostringstream oss;
        oss <<  "Name:  "   <<  GetName()   <<  ", Email: " <<  GetEmail();
        return oss.str();
    }

    BookReadingSession* AddBookReadingSession (Book* book) {
        BookReadingSession* session = new BookReadingSession(book);
        reading_sessions.push_back(session);
        return session;
    }

    const vector<BookReadingSession*>& GetBookReadingSessions () const{
        return reading_sessions;
    }

    void SetIsLibraryAdmin (const bool& is_admin) {
        this->is_admin = is_admin;
    }

    const bool& IsLibraryAdmin () const {
        return is_admin;
    }
};



/////////////////////User Manager/////////////////////////////
class UserManager{
private:
    map<string, User*> username_userobject_map;
    User* current_user{nullptr};

    void FreeLoadedData () {
        for(auto& pair : username_userobject_map)
            delete pair.second;
        username_userobject_map.clear();
    }
public:
    UserManager() = default;

    ~UserManager(){
        FreeLoadedData();
    }

    void LoadData () {
        if (username_userobject_map.size() > 0)
			return;		// let's assume 1 run for this system only

		cerr << "UsersManager: LoadDatabase\n";

		FreeLoadedData();
		// Some "Dummy Data" for simplicity
		User* user1 = new User();
		user1->SetUserName("mostafa");
		user1->SetPassword("111");
		user1->SetEmail("most@gmail.com");
		user1->SetIsLibraryAdmin(true);
		user1->SetName("Mostafa Saad Ibrahim");
		username_userobject_map[user1->GetUserName()] = user1;

		User* user2 = new User();
		user2->SetUserName("asmaa");
		user2->SetPassword("222");
		user2->SetEmail("asmaa@gmail.com");
		user2->SetIsLibraryAdmin(false);
		user2->SetName("Asmaa Saad Ibrahim");
		username_userobject_map[user2->GetUserName()] = user2;
    }


    void AccessSystem() {
        int choice = ShowReadMenu({"Login", "Sign Up"});

        if(choice == 1){
            DoLogIn();
        }else{
            DoSignUp();
        }
    }

    void DoSignUp () {
        string user_name{""};

        while(true){
            cerr    <<  "Enter User Name:   ";
            cin     >>  user_name;

            if(username_userobject_map.count(user_name)){
                cerr    <<  "Already in use user name, try again...\n";
            }else{
                break;
            }
        }

        current_user = new User();
        current_user->Read(user_name);
        username_userobject_map[user_name] = current_user;
    }

    void DoLogIn () {
        LoadData();

        string user_name, pass;

        while(true){
            cerr    <<  "Enter user name:   ";
            cin     >>user_name;

            if(username_userobject_map.count(user_name) == 0){
                cerr    <<  "Sorry, user name does not exist...\n";
                continue;
            }

            User* user_exist = username_userobject_map.find(user_name)->second;

            cerr    <<  "Enter password:    ";
            cin     >>pass;

            if(pass != user_exist->GetPassword()){
                cerr    <<  "Wrong password, please try again...\n";
                continue;
            }

            current_user = user_exist;
            break;
        }
    }

    BookReadingSession* AddBookReadingSession (Book* book) {
        return current_user->AddBookReadingSession(book);
    }

    const User* GetCurrentUser () const {
        return current_user;
    }
};



////////////////////Book Manager////////////////////////
class BookManager{
private:
    map<string, Book*> isbn_book_map;

    void FreeLoadedData () {
        for(auto& pair : isbn_book_map){
            delete pair.second;
        }
        isbn_book_map.clear();
    }

public:

    void LoadData () {
        FreeLoadedData();

		Book* book1 = new Book();
		book1->SetISBN("00001");
		book1->SetAuthorName("Mostafa");
		book1->SetTitle("C++ how to program");
		vector<string> pages1 = { "A C++", "B C++", "C C++", "D C++", "E C++" };
		book1->SetPages(pages1);
		AddBook(book1);

		Book* book2 = new Book();
		book2->SetISBN("00002");
		book2->SetAuthorName("Morad");
		book2->SetTitle("Intro to algo");
		vector<string> pages2 = { "A Algo", "B Algo", "C Algo", "D Algo", "E " };
		book2->SetPages(pages2);
		AddBook(book2);

		Book* book3 = new Book();
		book3->SetISBN("00003");
		book3->SetAuthorName("Morad");
		book3->SetTitle("Data Structures in C++");
		vector<string> pages3 = { "A Data", "B Data", "C Data", "D Data", "E Data" };
		book3->SetPages(pages3);
		AddBook(book3);
    }

    void AddBook (Book* book) {
        isbn_book_map[book->GetISBN()] = book;
    }

    void SetIsbnBookMap (const map<string, Book*>& isbn_book_map) {
        this->isbn_book_map = isbn_book_map;
    }

    const map<string, Book*>& GetIsbnBookMap () const {
        return isbn_book_map;
    }
};




///////////////////////User View//////////////////////////
class UserView {
private:
    UserManager& user_manager;
    BookManager& book_manager;

public:
    UserView(UserManager& user_manager, BookManager& book_manager) :
        user_manager(user_manager), book_manager(book_manager){
    }

    void Display () {
        const User* current_user = user_manager.GetCurrentUser();

        cerr    <<  "Name:  "   <<  current_user->GetName() <<  " | User-view\n";

        vector<string> menu;
        menu.push_back("View Profile");
		menu.push_back("List & Select from My Reading History");
		menu.push_back("List & Select from Available Books");
		menu.push_back("Logout");

		while(true){
            int choice = ShowReadMenu(menu);
            if(choice == 1){
                ViewProfile();
            }else if(choice == 2){
                ListReadingHistory();
            }else if(choice == 3){
                ListAvailableBooks();
            }else{
                break;
            }
        }
    }

    void ViewProfile () const {
        const User* current_user = user_manager.GetCurrentUser();
        cerr    <<  "\n"    <<  current_user->ToString()    <<  "\n";
    }

    void DisplaySession(BookReadingSession* session) {
        vector<string> menu;
		menu.push_back("Next Page");
		menu.push_back("Previous Page");
		menu.push_back("Stop Reading");

		while(true){
            cerr    <<  "Current Page: "    <<  session->PageIdx()  <<  "\n";
            cerr    <<  session->GetCurrentPageContent()  <<  "\n";

            int choice = ShowReadMenu(menu);
			if (choice == 1)
				session->NextPage();
			else if (choice == 2)
				session->PrevPage();
			else
				break;
		}

		session->ResetLastTime();
    }


    void ListReadingHistory () {
        int idx{0};
        const auto& sessions = user_manager.GetCurrentUser()->GetBookReadingSessions();
        for(;idx<(int)sessions.size(); idx++){
            cerr    << idx+1    <<  ")" <<  sessions[idx]->ToString()   <<  "\n";
        }

        if(idx == 0){
            cerr    <<  "Oops, it looks like you have not read before :)...\n";
        }else{
            cerr    <<  "Which session you want to continue reading at: \n";
            int choice = ReadInt(1, idx);
            DisplaySession(sessions[choice-1]);
        }
    }

    void ListAvailableBooks () {
        const map<string, Book*> mp = book_manager.GetIsbnBookMap();

        cout << "\nOur current book collection:\n";
		int idx = 0;
		for (const auto &pair : mp) {
			cout << ++idx << " " << pair.second->ToString() << "\n";
		}

		cout << "\nWhich book to read?: ";
		int choice = ReadInt(1, idx);

		idx = 0;
		for (const auto &pair : mp) {
			if (++idx == choice) {
				BookReadingSession* session = user_manager.AddBookReadingSession(pair.second);
				DisplaySession(session);
				break;
			}
		}
    }

};



//////////////////////////Administrator View///////////////////////////////
class AdminView{
private:
    UserManager& user_manager;
    BookManager& book_manager;

public:
    AdminView(UserManager& user_manager, BookManager& book_manager) :
        user_manager(user_manager), book_manager(book_manager){
    }

    void Display () {
        const User* current_user = user_manager.GetCurrentUser();

        cerr    <<  "Name:  "   <<  current_user->GetName() <<  " | Admin-view\n";

        vector<string> menu;
        menu.push_back("View Profile");
		menu.push_back("Add Book");
		menu.push_back("Logout");

		while(true){
            int choice = ShowReadMenu(menu);
            if(choice == 1){
                ViewProfile();
            }else if(choice == 2){
                AddBook();
            }else{
                break;
            }
        }
    }

    void ViewProfile () {
        const User* current_user = user_manager.GetCurrentUser();
        cerr    <<  "\n"    <<  current_user->ToString()    <<  "\n";
    }

    void AddBook() {
        Book *book = new Book();
		book->Read();
		book_manager.AddBook(book);
    }

};




/////////////////////////// On line Book Reader/////////////////////////////
class OnlineBookReader {
private:
    BookManager* book_manager{nullptr};
    UserManager* user_manager{nullptr};

    void LoadData () {
        book_manager->LoadData();
        user_manager->LoadData();
    }

public:
    OnlineBookReader () :
        book_manager(new BookManager()), user_manager(new UserManager()){

    }

    ~OnlineBookReader () {
        if (book_manager != nullptr) {
			delete book_manager;
			book_manager = nullptr;
		}
		if (user_manager != nullptr) {
			delete user_manager;
			user_manager = nullptr;
		}
    }

    void Run() {

        LoadData();

        while(true){
            user_manager->AccessSystem();

            if (user_manager->GetCurrentUser()->IsLibraryAdmin()) {
				AdminView view(*user_manager, *book_manager);
				view.Display();
			} else {
				UserView view(*user_manager, *book_manager);
				view.Display();
			}
        }
    }
};


int main () {
    OnlineBookReader site;

    site.Run();
}
