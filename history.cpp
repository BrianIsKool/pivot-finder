#include </usr/include/c++/11/iostream>
#include </usr/include/postgresql/libpq-fe.h>
#include <list>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <chrono>
#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>

using namespace std;

float tmp_price = 0;
int isCreated_tmp = 0;
double id_tmp = 0;

// list<array<double, 3>> zz;
// vector<std::vector<double>> levels;

struct Data_pivots {
    int id;
    std::string link;
    double price;
    char type;
};

void IsCreated(float a, float price, double id){
    if (isCreated_tmp == 0){
        tmp_price = price;
        isCreated_tmp = a;
        id_tmp = id;
    }
    
}
class Highs{
    private:
        Highs* link;
        double id;
        double price;
        
    public:
        Highs(Highs* link_, double id_, double price_) : link(link_), id(id_), price(price_){}

        Highs* get_link(){
            return link;
        }
        double get_price(){
            return price;
        }

        std::tuple<double, double> get_high() {
            return std::make_tuple(id, price);
        }
        void set_link(Highs* prev) {
            link = prev;
        }
        
};

class Lows{
    private:
        Lows* link;
        double id;
        double price;
        
    public:
        Lows(Lows* link_, double id_, double price_) : link(link_), id(id_), price(price_){}

        Lows* get_link(){
            return link;
        }
        double get_price(){
            return price;
        }

        std::tuple<double, double> get_low() {
            return std::make_tuple(id, price);
        }
        void set_link(Lows* prev) {
            link = prev;
        }
        
};

class Pivots{
    private:
        double id;
        double price;
        int type;
    public:
        Pivots* link;
        Pivots(Pivots* link_, double id_, double price_, int type_): link(link_), id(id_), price(price_), type(type_){}


        Pivots* get_link(){
            return link;
        }
        std::tuple<double, double, int> get_pivot() {
            return std::make_tuple(id, price, type);
        }
        void set_link(Pivots* prev) {
            link = prev;
        }

};


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <limitValue>" << std::endl;
        return 1;
    }
    int limitValue;
    std::istringstream(argv[1]) >> limitValue;
    std::stringbuf buffer;
    std::ostream os(&buffer);
    os << "SELECT open FROM btcfdusd_1s ORDER BY id desc limit " << limitValue;
    std::string formattedQuery = buffer.str();
    float sensitivity = 0.015/100;
    ofstream out;
    ofstream out2;
    out.open("levels_low1.csv");
    out2.open("levels_high1.csv");

    PGconn *conn = PQconnectdb("dbname= user= password= host= port=5432");

    // Выполняем SQL-запрос
    const char *query = formattedQuery.c_str();

    PGresult *result = PQexec(conn, query);
    const char* element0D = PQgetvalue(result, 0, 0);
    // cout << element0D << endl;
    int rows = PQntuples(result);
    int cols = PQnfields(result);
    double element0 = std::stod(element0D);

    Pivots* FirstObj = new Pivots(nullptr, 0, element0, 0);
    Pivots* pivot = FirstObj;

    Lows* FirstLow = new Lows(nullptr, 0, element0);
    Lows* low = FirstLow;

    Highs* FirstHigh = new Highs(nullptr, 0, element0);
    Highs* high = FirstHigh;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    for (double i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // std::cout << PQgetvalue(result, i, j) << "\t";
            const char* priceD = PQgetvalue(result, i, j);
            double price = std::stod(priceD);

            if (price > element0 - element0*sensitivity){ // находим high
                IsCreated(1, price, i);
                if (isCreated_tmp == 1){
                    if (price >= tmp_price){
                        tmp_price = price;
                        id_tmp = i;
                    }
                    else if (price + price*sensitivity < tmp_price){

                        Pivots* nextObj = new Pivots(pivot, id_tmp, price, 1); // 
                        nextObj->set_link(pivot);
                        pivot = nextObj;    

                        element0 = price;
                        tmp_price = 0;
                        isCreated_tmp = 0;
                        id_tmp = 0;
                        if (price < element0 + element0*sensitivity){
                            IsCreated(2, price, i);
                        }
                    }
                
                }   
            }
            else{
                if (isCreated_tmp == 1){
                    if (price + price*sensitivity < tmp_price){

                        Pivots* nextObj = new Pivots(pivot, id_tmp, price, 1);
                        nextObj->set_link(pivot);
                        pivot = nextObj;    
                     
                        tmp_price = 0;
                        isCreated_tmp =0;
                        id_tmp = 0;
                    }
                }
            }

            if (price < element0 + element0*sensitivity){ // находим low
                IsCreated(2, price, i);
                if (isCreated_tmp == 2){
                    if (price <= tmp_price){
                        tmp_price = price;
                        id_tmp = i;
                    }
                    else if (price + price*sensitivity > tmp_price){
                        
                        Pivots* nextObj = new Pivots(pivot, id_tmp, price, -1);
                        nextObj->set_link(pivot);
                        pivot = nextObj;                        

                        
                        element0 = tmp_price;
                        tmp_price = 0;
                        isCreated_tmp = 0;
                        id_tmp = 0;
                        if (price < element0 + element0*sensitivity){
                            IsCreated(1, price, i);
                        }
                    }
                
                }   
            }
            else{
                if (isCreated_tmp == 2){
                    if (price + price*sensitivity > tmp_price){

                        Pivots* nextObj = new Pivots(pivot, id_tmp, price, -1);
                        nextObj->set_link(pivot);
                        pivot = nextObj;    

                        element0 = tmp_price;
                        tmp_price = 0;
                        isCreated_tmp =0;
                        id_tmp = 0;
                    }
                }
            }

            // CurrentObj = pivot;
            // cout << pivot.get_link() << endl;
            // auto lin = pivot.get_link()->get_pivot();
            
            auto [idZ, priceZ, typeZ] = pivot->get_pivot();
            // std::cout << "ID: " << idZ << ", Price: " << priceZ << ", Type: " << typeZ << std::endl;

            if (typeZ == 1){ // находим хаи
                if(priceZ > high->get_price()){
                    Highs* nextHigh = new Highs(high, idZ, priceZ);
                    nextHigh->set_link(high);
                    high = nextHigh;    
                    
                }
            }
            if (typeZ == -1){ // находим лои
                if(priceZ > low->get_price()){
                    Lows* nextLow = new Lows(low, idZ, priceZ);
                    nextLow->set_link(low);
                    low = nextLow;    
                    
                }
            }
            Lows* fw = low; 
            int step = 0;
            out << "" << i;
            while (fw->get_link() != nullptr && step <= 2){
                // cout << fw->get_price() << " ";
                out << " " << fw->get_price() << "";
                fw = fw->get_link();
                step++;
            }
            out << "\n";

            Highs* fwh = high; 
            int step_h = 0;
            out2 << "" << i;
            while (fwh->get_link() != nullptr && step_h <= 2){
                // cout << fwh->get_price() << " ";
                out2 << " " << fwh->get_price() << "";
                fwh = fwh->get_link();
                step_h++;
            }
            out2 << "\n";


            }
    }


    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Time taken by function: " << duration.count() << " ms" << std::endl;
    // Освобождаем ресурсы
    PQfinish(conn);
    PQclear(result);
    return 0;
}