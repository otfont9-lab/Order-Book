#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <cstdint>
#include <list>
#include <unordered_map>
#include <chrono>
// This program simulates an orderbook. The orderbook stores the buy and sell orders in a private list , and fills them
// The best prices are filled once, and within the same price level, the earlier orders are before fulfilled.  


enum SIDE {
    BUY , 
    SELL
}; 

// An order is defined using the struct method,and keeps the most important features of an order 
struct Order  { 
uint64_t id ; 
double price; 
int volume ; 
SIDE side ; 
uint64_t  timestamp;
}; 




// The orderbook class is defined as follows. The orders are classified in bids and asks.
class Orderbook { 
private : 
    std :: map <double , std::list<Order>> asks ; 
    std :: map <double , std::list<Order> , std::greater<double>> bids ; 

    std :: unordered_map<uint64_t , std::list<Order>:: iterator> order_lookup ; 

public : 

// The addorder function hosts the logic flow of order fullfilment.  
void addOrder ( Order& order) { 
    // Splits the logic in two ; if it's either a buy or sell order. 

    if(order.side== BUY)   {  
        while( order.volume> 0 && !asks.empty() && order.price >= asks.begin()-> first) {                //The loop works while the current buy order can fulfill, toally or partially, any current ask, that will be, naturally, the best ask  
           
            auto & best_ask_list= (asks.begin() ->second) ;                              // Seeks the earliest order at best price
             if (best_ask_list.empty()) {
                asks.erase(asks.begin()); // Clean up the empty price level
                continue;
            }
            auto& best_ask_first_order = best_ask_list.front();
            
            int traded_volume = std :: min (best_ask_first_order.volume , order.volume);               // Prints the details of the operation
            std::cout << "[TRADE] BUY ORDER " << order.id
            <<" matched with SELL ORDER" << best_ask_first_order.id 
            << "Volume traded" << traded_volume 
            << "@ Price : $" << asks.begin()->first ; 
            
            if ( best_ask_first_order.volume <  order.volume ) {          // First case , the order fulfills totally the best ask order
                
                order.volume -= best_ask_first_order.volume ;              //The volume of the order is updated
                (asks.begin()-> second).pop_front();                            // The ask order is deleted
                if (asks.begin()->second.empty()) {
                asks.erase(asks.begin());                                  // If doesn't exist any  other  ask order at the same price , the 
                } 
               order_lookup.erase(best_ask_first_order.id);
            }
            else {                                                      //Second case,the order is only partially fulfilled
                best_ask_first_order.volume -=  order.volume ;
                order.volume = 0 ;    
            }
        }
        if(order.volume > 0) {                                        // If there's remaining volume at the end , add the order at the order list.
            bids[order.price ].push_back(order) ; 
            }
            order_lookup[order.id] = std::prev(bids[order.price].end());
        }
    else {                                                                                  //Same logic for an ask order
        while( order.volume> 0 && !bids.empty() && order.price <= bids.begin()-> first) {  
            auto & best_bid_list= (bids.begin() ->second) ;                              // Seeks the earliest order at best price
             if (best_bid_list.empty()) {
                bids.erase(bids.begin()); // Clean up the empty price level
                continue;
            }
            auto& best_bid_first_order = best_bid_list.front();
            int traded_volume = std :: min (best_bid_first_order.volume , order.volume); 
            std::cout << "[TRADE] SELL ORDER " << order.id
            <<" matched with BUY ORDER" << best_bid_first_order.id 
            << "Volume traded" << traded_volume 
            << "@ Price : $" << bids.begin()->first ; 

            
            if ( best_bid_first_order.volume <  order.volume ) {
                (bids.begin()-> second).pop_front();
                order.volume -=  best_bid_first_order.volume ;
                if (bids.begin()->second.empty()) {
                bids.erase(bids.begin());
                }
                order_lookup.erase(best_bid_first_order.id); 
            }
            else {  
            best_bid_first_order.volume -=  order.volume ;
            order.volume = 0 ; 
            }
        }
        if(order.volume > 0) {
            asks[order.price ].push_back(order) ; 
            order_lookup[order.id] = std::prev(asks[order.price].end());
        } 
    }
    
    } //The printbook function prints all th edetails of the orderbook 
void Printbook() const {  
    std::cout<< "\n==========================ORDER BOOK =================================\n";

// Print Asks ( Sellers)
    std:: cout << "\n=======================ASKS=============================\n";

    if (asks.empty()) {
        std::cout << "\n===================NO ASKS=============================\n";
    } else {
    for (const auto & [price , orderList] : asks ) {
        std :: cout << "Price $ :"<< price;
        for (const auto & order : orderList) {
        std:: cout << "[Id: " << order.id << "Volume: "<<order.volume<< "]";

        }
    std:: cout<< "\n";
    }
}

    std :: cout<<"----------------------------------------------------------\n";

// Print Bids ( Buyers)
    std:: cout << "\n=======================BIDS=============================\n";

    if (bids.empty()) {
        std::cout << "\n===================NO BIDS=============================\n";
    } else {
    for (const auto & [price , orderList] : bids ) {
        std :: cout << "Price $ :"<< price;
        for (const auto & order : orderList) {
        std:: cout << "[Id: " << order.id << "Volume: "<<order.volume<< "]";
        }
    std:: cout<< "\n";
    }
}
    std :: cout << "=================================================================\n\n";
} 
bool CancelOrder (int orderid) {                                        //Cancells an order in time 0(1) due the use of hashmap, that maps Order Ids to iterators
    auto lookup_IT = order_lookup.find(orderid);
    if ( lookup_IT == order_lookup.end()) {
    std::cout << "[CANCEL] ORDER ID "<<orderid << "Order not found"; 
    return false ; }

    auto listIT = lookup_IT-> second; 
    double price = listIT-> price ;
    SIDE  side = listIT-> side ; 


    if (side == BUY) {
        auto mapIT = bids.find(price);
        if ( mapIT != bids.end()) {
            mapIT->second.erase(listIT) ;
            if (mapIT->second.empty()){
                bids.erase(mapIT);
            }
        }   
    }else {
        auto mapIT = asks.find(price); 
        if(mapIT != asks.end()){
            mapIT->second.erase(listIT); 
            if (mapIT->second.empty()){
                asks.erase(mapIT); 
            }
        }
    }
}
};


int main() {
    Orderbook book;

    std::cout << "Starting simulation of 100,000 orders...\n";

    // Start the stopwatch
    auto start = std::chrono::high_resolution_clock::now();

    // Flood the engine with one hundred thousand of  orders
    for (uint64_t i = 1; i <= 100'000; ++i) {
        // Alternate between BUY and SELL
        SIDE side = (i % 2 == 0) ? BUY : SELL;
        
        // Keep prices clustered around 100 so they actually match each other
        double price = 100.0 + (i % 20); 
        int volume = 10;

        Order ord{i, price, volume, side, i};
        book.addOrder(ord);
    }

    // Stop the stopwatch
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate elapsed time in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "----------------------------------------\n";
    std::cout << "Successfully processed 100,000 orders!\n";
    std::cout << "Total time: " << duration << " ms\n";
    std::cout << "----------------------------------------\n";

    // Optional: Test O(1) cancellation speed
    // Let's cancel an order in the middle of the book
    auto cancel_start = std::chrono::high_resolution_clock::now();
    book.CancelOrder(50'000); 
    auto cancel_end = std::chrono::high_resolution_clock::now();
    
    auto cancel_duration = std::chrono::duration_cast<std::chrono::microseconds>(cancel_end - cancel_start).count();
    std::cout << "Cancel operation took: " << cancel_duration << " microseconds.\n";

    return 0;

};

