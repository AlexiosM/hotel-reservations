#include "hotel.hpp"

int main()
{   // name arrival days persons_num
    Reservation holidays("Alex Mantas", 4, 5, 2);
    Reservation holidays2("bla bla", 2 , 9, 5);

    std::cout << holidays;
    std::cout << holidays2;


    Room alexRoom;
    if(!alexRoom.addNewReservation(&holidays)) std::cout << "smth went wrong";

    alexRoom.cancelReservation(1);

    if(!alexRoom.addNewReservation(&holidays2)) std::cout << "smth went wrong";

    std::cout << "\nCost : " << alexRoom.summaryCostComputation() << std::endl;

    std::cout << "\nPercentage : " << alexRoom.reservationPercentage() << std::endl;


    std::cout << "-----------------------------\n\n\n\n\n";


    Reservation res("foufoutos", 1, 7, 2);
    RoomTypeB roomB;

    roomB.addNewReservation(&res);

    std::cout << "SumCost: " << roomB.summaryCostComputation() << std::endl;

    std::cout << "-----------------------------\n\n\n\n\n";

    Hotel myhotel;
    myhotel.addNewRoom(alexRoom);

    Room * r = myhotel.retrieveRoomById(1);

    std::cout << "perc: " << r->reservationPercentage() << std::endl;
    std::cout << "sum cost: " << r->summaryCostComputation() << std::endl;

    std::cout << "-----------------------------\n\n\n\n\n";

    Reservation reserv1("ruby",2,2,2);

   myhotel.addReservationToAnyRoom(reserv1);




    return 0;
}
