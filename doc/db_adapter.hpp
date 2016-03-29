User:
    int id;
    str login;
    str password;
//---------------
Room:
    int id;
    str name;
//---------------
User-Room:
    int id;
    int user_id;
    int room_id;
//---------------
Message:
    int id;
    int user_id; // sender
    int room_id;
    str text;
    Time time;
    str theme;
    bool read;
//---------------
Image:
    int id;
    int room_id;
//---------------
DBAdapter:

    bool open();
    void close();

    bool is_user_exist(int user_id);
    bool is_user_exist(str login);
    bool is_room_exist(int room_id);

    int register_user(str login, str password);
    int register_room(str name);
    int register_message(Message message);
    int register_image(Image image);

    void change_user_password(int user_id, str new_password);
    void change_room_name(inr room_id, str name);

    str get_user_login(int user_id);
    int get_user_id(str login);

    str get_room_name(int room_id);

    bool check_user(str login, str password);

    [int] get_user_rooms(int user_id);
    [int] get_users_in_room(int room_id);
    bool is_user_in_room(int user_id, int room_id);
    void add_user_to_room(int user_id, int room_id);

    [Message] get_messages_in_room(int room_id, ??filter??);

    int get_image_room(int image_id);

    void mark_message(int message_id); //set read flag to message

// Память под массивы и объекты выделяет адаптер. Можно возвращать только указателя на них. Сервер освободит память.
// Нужно подумать над форматом фильтра для сообщений. Например: все сообщения, последние N сообщений во всех ветках, сообщения после определенного времени и т.д.
// Нужно решить, каким будет формат времени.
