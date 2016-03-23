
#define OK 1
#define ERROR 0

class User {


private:

    int user_id;
    char *login;
    char *password;

}

class DB_Adapter {

public:

    // Этот момент надо расширить 
    DB_Adapter( char *database_name );

// Работа с пользователями
// -----------------------------------------------------------------------------
    User *getUserByID( int user_id );        // Получения пользователя по id
    // Проверка авторизации
    bool checkAuth( int user_id, char *password );
    bool checkAuth( char *login, char *password );

    bool addUser( User user );            // Добавление пользователя в бд

// -----------------------------------------------------------------------------

// Работа с сообщениями
// -----------------------------------------------------------------------------

    Message *getMessageByID( int message_id );

    // Если на вход подаётся size = -1, то выбираются 40 сообщений.
    // Иначне по числу в size. По числу from опеределяется с какого сообщения
    // по добавлению будет выборка. 0 - с последнего.
    // 1 - с предпоследнего и т.д.
    Message *getRoomMessages( int room_id, size_t &size, size_t from = 0 );
    // По всем комнатам, в которых есть юзер
    // Например, сначала вызов функции getUserRooms, а потом getRoomMessages
    // к каждой
    Message *getUserMessages( int user_id, size_t &size, size_t from = 0 );
    // Тоже самое, но возращается только массив id-шек
    int *getRoomMessagesIDs( int room_id, size_t &size, size_t from = 0 );
    int *getUserMessagesIDs( int user_id, size_t &size, size_t from = 0 );

    bool addMessage( Message message );
    // Обновляет данные в переменной сообщения
    // Возврат true - если что-то изменилось
    bool upDateMessage( Message &message );
// -----------------------------------------------------------------------------

// Работа с комнатой
// -----------------------------------------------------------------------------
    Room *getRoomByID( int room_id );

    bool addRoom( Room room );

    Room *getUserRooms( int user_id, size_t &size, size_t from = 0 );
    //int *getUserRoomsIDs( int user_id, size_t &size, size_t from = 0 );

// -----------------------------------------------------------------------------


}
