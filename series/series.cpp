#include <iostream>
#include <algorithm>    // std::sort
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator> // for iterators 

using namespace std;

class Book{

private:
    int book_id;
    string book_title;
    string genre_1;
    string genre_2;
    int pages;
    string author_name;
    double author_average_rating;
    double popularity;

public:
    Book(int _book_id, string &_book_title, string &_genre_1, string &_genre_2, int _pages, string &_author_name, double _author_average_rating){
        book_id = _book_id;
        book_title = _book_title;
        genre_1 = _genre_1;
        genre_2 = _genre_2;
        pages = _pages;
        author_name = _author_name;
        author_average_rating = _author_average_rating;
    }

    double get_popularity(){
        return popularity;
    }
    void set_popularity(double _popularity){
        popularity = _popularity;
    }
    int get_book_id(){
        return book_id;
    }
    // void set_book_id(int _book_id){
    //     book_id = _book_id;
    // }
    string get_book_title(){
        return book_title;
    }
    // void set_book_title(string _book_title){
    //     book_title = _book_title;
    // }
    string get_genre_1(){
        return genre_1;
    }
    // void set_genre_1(int _genre_1){
    //     genre_1 = _genre_1;
    // }
    string get_genre_2(){
        return genre_2;
    }
    // void set_genre_2(int _genre_2){
    //     genre_2 = _genre_2;
    // }
    int get_pages(){
        return pages;
    }
    // void set_pages(int _pages){
    //     pages = _pages;
    // }
    string get_author_name(){
        return author_name;
    }
    // void set_pages(string _author_name){
    //     author_name = _author_name;
    // }
    double get_author_average_rating(){
        return author_average_rating;
    }
    // void set_pages(double _author_average_rating){
    //     author_average_rating = _author_average_rating;
    // }
};

void split(vector<string> &arguments, string &line){
    stringstream mystringstream(line);
    string segment;

    while(getline(mystringstream, segment, ','))
    {
       arguments.push_back(segment);
    }
}

class Review{
public:
	Review(int _book_id, int _rating, int _number_of_likes){
		book_id = _book_id;
		rating = _rating;
		number_of_likes = _number_of_likes;
	}
	int get_book_id(){
		return book_id;
	}
	int get_rating(){
		return rating;
	}
	int get_number_of_likes(){
		return number_of_likes;
	}
private:
	int book_id;
	int rating;
	int number_of_likes; 
};

bool review_compare(Review &review_1, Review &review_2){
	return (review_1.get_book_id() < review_2.get_book_id()); 
}

void review_store(vector<Review> &reviews){
    string line;
    ifstream myfile ("reviews.csv");
    if (myfile.is_open())
    {
  		getline(myfile, line);
        while ( getline (myfile, line) )
        {
            vector<string> arguments;
            split(arguments, line);
        	Review review(stoi(arguments[0]), stoi(arguments[1]), stoi(arguments[2]));
        	reviews.push_back(review);
        }
        myfile.close();
    }

    sort(reviews.begin(), reviews.end(), review_compare);
}

bool operator<(int v, Review &foo)
{ 
	return v < foo.get_book_id();
}

bool operator<(Review &foo, int v)
{
	return foo.get_book_id() < v;
}

void store(vector<Book> &books, vector<Review> &reviews, vector<string> &arguments){
    Book book(stoi(arguments[0]), arguments[1], arguments[2], arguments[3], stoi(arguments[4]), arguments[5], stod(arguments[6]));

    int sum = 0, total_books_reviews_likes = 0;
	pair<vector<Review>::iterator, 
	              vector<Review>::iterator> ip; 
    ip = equal_range(reviews.begin(), reviews.end(), book.get_book_id());
  	vector<Review>::iterator current_iterator;
  	for(current_iterator = ip.first; current_iterator != ip.second; ++current_iterator){
        sum += current_iterator -> get_rating() * current_iterator -> get_number_of_likes();
        total_books_reviews_likes += current_iterator -> get_number_of_likes();
  	}
    
    double fraction;
    if(total_books_reviews_likes == 0)
    	fraction = 0;
    else
    	fraction = sum/(double)total_books_reviews_likes;

	book.set_popularity(0.1 * (book.get_author_average_rating() + fraction));    
    books.push_back(book);
}

void book_handler(vector<Book> &books, vector<Review> &reviews, string &line){
    vector<string> arguments;
    split(arguments, line);
    store(books, reviews, arguments);
}

void readFile(vector<Book> &books){
  vector<Review> reviews;
  review_store(reviews);

  string line;
  ifstream myfile ("books.csv");
  if (myfile.is_open())
  {
  	getline(myfile, line);
    while ( getline (myfile, line) )
    {
        book_handler(books, reviews, line);
    }
    myfile.close();
  }
}

bool compare(Book &book_1, Book &book_2) 
{ 
    return (book_1.get_popularity() < book_2.get_popularity()); 
} 

int main(int agrc, char *argv[]){
    vector<Book> books;
    readFile(books);
    string genre = argv[1];
    vector<Book> returnValue;

    for(int i = 0; i < books.size(); ++i){
        if(books[i].get_genre_1() == genre || books[i].get_genre_2() == genre)
            returnValue.push_back(books[i]);
    }
    
    sort(returnValue.begin(), returnValue.end(), compare);

    cout << "id: "
    << returnValue.back().get_book_id()
    << endl
    << "Title: "
    << returnValue.back().get_book_title()
    << endl
    << "Genres: " 
    << returnValue.back().get_genre_1() << ", " << returnValue.back().get_genre_2()
    << endl
    << "Number of Pages: "
    << returnValue.back().get_pages()
    << endl
    << "Author: "
    << returnValue.back().get_author_name()
    << endl
    << "Average Rating: "
    << returnValue.back().get_popularity()
    << endl;

    return 0;
}