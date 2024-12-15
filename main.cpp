#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

struct Book;
using namespace std;
int BOOK_ID = 1;
int USER_ID = 1;


// User struct : name, id, borrowed books
struct User {
    int id;
    string name;
    vector<Book> borrowedBooks;
};
// Book struct : title, id, users who borrow the book, quantity
struct Book {
    int id;
    string name;
    int quantity;
    vector<User> users_borrowed_the_book;

    bool operator==(const Book& other) const {
        return (name == other.name && id == other.id);  // Compare by title (or any other unique identifier)
    }
};

class LibraryController {
    public:
    bool isAdmin = true;
    LibraryController(){};
    LibraryController(bool isAdmin) {
        this->isAdmin = isAdmin;
    };

    void addBook (string title, int quantity, vector<Book>& books) {
        Book newBook = {
            BOOK_ID++,
            title,
            quantity
        };
            books.push_back(newBook);
    }

    void addUser(string name, vector<User>& users) {
        User newUser = {USER_ID++, name};
            users.push_back(newUser);
    }

    void borrowBook(User& user, Book& book, vector<Book>& books) {
        auto it = find(books.begin(), books.end(), book);
        if( it != books.end()) {
            // Found
            if (book.quantity > 0) {
                user.borrowedBooks.push_back(book);
                book.quantity--;
                book.users_borrowed_the_book.push_back(user);
            } else {
                cout << "Book run out!" << endl;
            }
        }
    }

    void returnBook (User& user, Book& book, vector<Book>& books) {
        if(find(books.begin(), books.end(), book) == books.end()) {
            // Not found
            cout << "Book not found!" << endl;
        } else {
            user.borrowedBooks.erase(find(user.borrowedBooks.begin(), user.borrowedBooks.end(), book));
            book.quantity++;
        }
    }

    void print_user(User user) {
        cout << "Name: " << user.name << endl;
        cout << "ID: " << user.id << endl;
        cout << "Boorowed books: " << endl;
        for (int i = 0; i < user.borrowedBooks.size(); i++) {
            cout << i+1 << ") " << user.borrowedBooks[i].name << endl;
        }
        cout << "--------------" << endl;
    }

    auto getBookById(int bookID, vector<Book>& books) {
        auto bookItr = find_if(books.begin(), books.end(), [bookID](Book& book)-> bool {
                   return bookID == book.id;
        } );
        return bookItr;
    }

    auto getUserById(int UserID, vector<User>& users) {
        auto userItr = find_if(users.begin(), users.end(), [UserID](User& user)-> bool {
                   return UserID == user.id;
        } );
        return userItr;
    }

    auto searchBookByPrefix(string prefix, vector<Book>& books, vector<Book>::iterator beginItr) {
        transform(prefix.begin(), prefix.end(), prefix.begin(), ::tolower);

        auto it = find_if(beginItr, books.end(), [prefix](Book& book)->bool {
            string lowerCaseTitle = book.name;
            transform(lowerCaseTitle.begin(), lowerCaseTitle.end(), lowerCaseTitle.begin(), ::tolower);
            return lowerCaseTitle.substr(0, prefix.size()) == prefix;
        });

        return it;
    }

    vector<Book> sortById(vector<Book> books) {
        sort(books.begin(), books.end(), [](Book& book1, Book& book2) {
            return book1.id < book2.id;
        });
        return books;
    }
    vector<Book> sortByName(vector<Book> books) {
        sort(books.begin(), books.end(), [](Book& book1, Book& book2) {
            return book1.name < book2.name;
        });
        return books;
    }
};

template<typename Type>
Type GetInput(string inputText) {
    cout << "> " << inputText << ": ";
    Type inputValue;
    cin >> inputValue;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Again! " << "> " << inputText << ": ";
        cin >> inputValue;
        cout << endl;
    }
    return inputValue;
}

template<>
string GetInput<string>(string inputText) {
    cout << "> " << inputText << ": ";
    string inputValue;
    cin.ignore();
    getline(cin, inputValue);
    while (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Again! " << "> " << inputText << ": ";
        getline(cin, inputValue);
        cout << endl;
    }
    return inputValue;
}

bool FIND_BOOK_BY_ID(Book book, int book_id) {
    return book.id == book_id;
}


int main() {
    // Users vector
    vector<User> users;
    // Books vector
    vector<Book> books;
    //exit flag
    bool exit = false;

    LibraryController controller(true);

    while (!exit) {
        cout << "1. Add Book" << endl;
        cout << "2. Add User" << endl;
        cout << "3. Return Book" << endl;
        cout << "4. Borrow Book" << endl;
        cout << "5. Print Users" << endl;
        cout << "6. Who borrow a book?" << endl;
        cout << "7. Search books by prefix" << endl;
        cout << "8. Print library by id" << endl;
        cout << "9. Print library by name" << endl;
        cout << "10. Exit" << endl;
        int choice = GetInput<int>("Choice");
        switch (choice) {
            // Add book
            case 1: {
                string bookTitle = GetInput<string>("Book Title");
                int bookQuantity = GetInput<int>("Book Quantity");
                controller.addBook(bookTitle, bookQuantity, books);
                break;
            }
            // Add user
            case 2: {
                string userName = GetInput<string>("Name");
                controller.addUser(userName, users);
                break;
            }
            // Return book
            case 3: {
                int userID = GetInput<int>("User ID");
                int bookID = GetInput<int>("Book ID");
                auto bookItr = controller.getBookById(bookID, books);
                auto userItr = controller.getUserById(userID, users);
                if (bookItr == books.end()) {
                    // not found
                    cout << "Book not found!" << endl;
                } else {
                    controller.returnBook(*userItr, *bookItr, books);
                }
                break;
            }
            // Borrow Book
            case 4: {
                int userID = GetInput<int>("User ID");
                int bookID = GetInput<int>("Book ID");
                auto bookItr = controller.getBookById(bookID, books);
                auto userItr = controller.getUserById(userID, users);
                if (bookItr == books.end()) {
                    // not found
                    cout << "Book not found!" << endl;
                } else {
                    controller.borrowBook(*userItr, *bookItr, books);
                }
                break;
            }
            // Print Users
            case 5: {
                for (User user : users) {
                    controller.print_user(user);
                }
                break;
            }
            // Who borrow a book?
            case 6: {
                int bookID = GetInput<int>("Book ID");
                auto bookItr = controller.getBookById(bookID, books);
                for (int i = 0; i < bookItr->users_borrowed_the_book.size(); i++) {
                    cout << i+1 << ") " << bookItr->users_borrowed_the_book[i].name << " " << bookItr->users_borrowed_the_book[i].id << endl;
                }
                break;
            }
            // Search books by prefix
            case 7: {
                string prefix = GetInput<string>("Book name");
                vector<Book>::iterator it = books.begin() - 1;
                while (it != books.end() - 1) {
                    it= controller.searchBookByPrefix(prefix, books, it+1);
                    cout <<"Name: " <<  it->name << " ID: "  <<  it->id << endl;
                }
                cout << "-----------" << endl;
                break;
            }
            //Print library by id
            case 8: {
                auto sorted = controller.sortById(books);
                for (int i = 0; i < sorted.size(); i++) {
                    cout << i+1 << ") " << sorted[i].name << endl;
                }
                cout << "-----------" << endl;
                break;
            }
            //Print library by name
            case 9: {
                auto sorted = controller.sortByName(books);
                for (int i = 0; i < sorted.size(); i++) {
                    cout << i+1 << ") " << sorted[i].name << endl;
                }
                cout << "-----------" << endl;
                break;
            }
            case 10: {
                exit = true;
                break;
            }
            default: {
                cout << "Invalid Choice!" << endl;
                break;
            }
        }
    }
}
