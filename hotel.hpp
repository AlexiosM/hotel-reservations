#include <iostream>
#include <string>
#include <array>
#include <string>
#include <vector>

class Room;

class Reservation
{
    private:

        std::string customer_name;
        int arrival;
        int accomodation_days;
        int persons;
        unsigned int reservation_id = 0;
        Room * room = nullptr;

    public:
        static unsigned int global_reservation_id;

        Reservation()
        {
            reservation_id = global_reservation_id++;
        }

        Reservation(std::string name,int arriv,int ac_days,int pers)
        {
            reservation_id = global_reservation_id++;
            customer_name = name;
            arrival = arriv;
            accomodation_days = ac_days;
            persons = pers;
        }

        void assign_Room(Room *r)
        {
            room = r;
        }
        
        unsigned int getId(void){
            return reservation_id;
        }

        int getArrival(void){
            return arrival;
        }
        
        int getDays(void){
            return accomodation_days;
        }

        int getPersonsNumber(void){
            return persons;
        }

        friend std::ostream& operator<< (std::ostream& os, const Reservation& res);
};

unsigned int Reservation::global_reservation_id = 0;

std::ostream& operator<< (std::ostream& os, const Reservation& res)
{
   os  <<  "\nRes. ID : " << res.reservation_id << 
       " from : " << res.customer_name  << " for " << res.persons << 
       " number of persons, that arive at " << res.arrival <<" of the month " << 
       " and will stay for " << res.accomodation_days << 
       " days.\nWhich means from " << res.arrival << " to " << res.accomodation_days+res.arrival << " of the month.\n\n";

    return os;
}


class Room
{
    private:
        float cost_per_person = 10.0;

    protected:
        std::array<Reservation *,30> availability_table = {nullptr};
        unsigned int room_code;
        int max_room_capacity = 10;
        Reservation* res;
        
    public:
        
        static unsigned int global_room_code;

        Room()
        {
            room_code = global_room_code++;
        }
        
        virtual bool addNewReservation(Reservation* reservation)
        {
            res = reservation;
            std::cout << "\nNew reservation request for " << res->getPersonsNumber() << " persons" 
                    << "\nArrival at " << res->getArrival() << " of the month" << std::endl;
            if ( res->getPersonsNumber() >= max_room_capacity)
                return false;

            for (int i=res->getArrival(); i < res->getArrival()+res->getDays(); i++){
                if (availability_table[i] != nullptr) {
                    std::cout << "Conflict! Already Reserved\n";
                    return false;
                }
            }

            for (int i=res->getArrival(); i < res->getArrival()+res->getDays(); i++){
                std::cout << "Reservation for day: " << i << " done successfully" << std::endl;
                availability_table[i] = res;
            }
            std::cout << "Reservation done with id : " << res->getId() << std::endl;

            res->assign_Room(this);

            return true;
        }

        virtual float summaryCostComputation(void)
        {
            for ( auto &element : availability_table ){
                if (element != nullptr) {
                    return static_cast<float> (element->getPersonsNumber() * element->getDays() * cost_per_person);
                }
            } 
            return (float)0;
        }

        bool cancelReservation(int reserv_id)
        {
            std::cout << "\nCanceling res. id: " << reserv_id << std::endl;
            for (int i =0; i < availability_table.size(); i++){
                if(availability_table[i] != nullptr){
                    if (availability_table[i]->getId() == reserv_id){
                        std::cout << "Reservation with id:" << availability_table[i]->getId() << "has been cancled\n";
                        availability_table[i] = nullptr;
                    }
                }
            }
            return true;
        }

        float reservationPercentage(void)
        {
            int count = 0;
            for (int i =0; i < availability_table.size(); i++){
                if(availability_table[i] != nullptr){
                    count++;
                }
            }
            return (static_cast<float>(count))/availability_table.size() * 100;
        }

        unsigned int getRoomCode(void)
        {
            return room_code;
        }
};

unsigned int Room::global_room_code = 1;

class RoomTypeA : public Room
{
    public:

        float summaryCostComputation(void) // overwritten method
        {
            return costPerDay * res->getDays();
        }


    protected:

        float costPerDay = 50;

};


class RoomTypeB : public RoomTypeA
{
    public:

        float summaryCostComputation(void) // overwritten method
        {
            float sumCost = 0;
            float currentDayCost = 0;

            for(int i=0; i < res->getDays(); i++) {
                currentDayCost = costPerDay - i*discountPerDay;
                if (currentDayCost > costPerDay/2) {
                    sumCost = sumCost + currentDayCost;
                }
                else {
                    sumCost = sumCost + costPerDay/2;
                }
            }
            return sumCost;
        }

        bool cancelReservation(int reserv_id)
        {
            return false;
        }
        
    private:
        float discountPerDay = 10;

};


class RoomTypeC : public Room
{
    private:
        int minimumDays;
        int minimumPersons;

    public:

        bool addNewReservation(Reservation* reservation)
        {
            res = reservation;
            std::cout << "\nNew reservation request for " << res->getPersonsNumber() << " persons" 
                    << "\nArrival at " << res->getArrival() << " of the month" << std::endl;
            if ( res->getPersonsNumber() >= max_room_capacity || res->getPersonsNumber() < minimumPersons   )
                return false;

            if ( res->getDays() < minimumDays )
                return false;

            for (int i=res->getArrival(); i < res->getArrival()+res->getDays(); i++){
                if (availability_table[i] != nullptr) {
                    std::cout << "Conflict! Already Reserved\n";
                    return false;
                }
            }

            for (int i=res->getArrival(); i < res->getArrival()+res->getDays(); i++){
                std::cout << "Reservation for day: " << i << " done successfully" << std::endl;
                availability_table[i] = res;
            }
            std::cout << "Reservation done with id : " << res->getId() << std::endl;

            res->assign_Room(this);

            return true;
        }




};

// ToDo
// add two new classes that inherrit from Room class implicitly or explicitly, and implement
// differnetly one or more of its methods
//


class Hotel
{
    public:
        std::string hotel_name;
        std::vector <Room> rooms;
        std::vector <Reservation> reservations;


        void addNewRoom(Room newRoom)
        {
            rooms.push_back(newRoom);
        }

        Room* retrieveRoomById(unsigned int room_id)
        {
            for (auto &r : rooms) {
                if(r.getRoomCode() == room_id)
                    return &r;
            }
            return nullptr;
        }

        Reservation* retrieveReservationById(unsigned int reservation_id)
        {
            for (auto &r : reservations){
                if (r.getId() == reservation_id)
                    return &r;
            }
            return nullptr;
        }


        bool addReservationToRoom(Reservation res, unsigned int room_id)
        {
            Room *r = retrieveRoomById(room_id);
            if (r != nullptr) // room exists
                if(r->addNewReservation(&res)) { // room available
                    reservations.push_back(res);
                    return true;
                }
            else
                return false;
        }

        unsigned int addReservationToAnyRoom(Reservation res)
        {
            for(auto &room : rooms){
                if(room.addNewReservation(&res)) { // this room is available
                    reservations.push_back(res); // add to done reservations                    
                    std::cout << "Reservation with id: " << res.getId()
                        << " done successfully, at room " << room.getRoomCode() << std::endl;
                    return room.getRoomCode();
                }
            }
            std::cout << "Reservation Failed" << std::endl;
            return 0;
        }

        void cancelReservation(unsigned int reservation_id)
        {
            Reservation *r;
            int reservation_index = 0;
            r = retrieveReservationById(reservation_id);
            // erase reservation from vector
            for (auto &res : reservations) { // find the index of the element
                if(reservation_id == res.getId()) {
                    reservation_index++;
                    break; 
                }
                reservation_index++;
            }
            reservations.erase(reservations.begin()+reservation_index);
            // set reservation null in the relevant element of room vector
            for (auto &room : rooms) { 
                if ()

            }


        }



};

