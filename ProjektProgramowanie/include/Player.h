/** \author Mateusz Miozga
 *
 * \file Player.h
 *
 * \brief Klasa reprezentująca gracza w grze
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <cstddef>
#include "Object.h"

/** \class Player
 *
 *  \brief Klasa gracza zawierająca statystyki, pozycję, broń i pociski
 */
class Player {
public:

    /** \brief Konstruktor - inicjacja gracza
     *
     * \param health int - liczba punktów zdrowia
     * \param attack int - obrażenia zadawane pocisku
     *
     */
    Player(int health, int attack);

    /** \brief getter
     *
     * \return int - aktualne zdrowie gracza
     *
     */
    int getHealth() const;
    /** \brief getter
     *
     * \return int - obrażenia gracza
     *
     */
    int getAttack() const;

    /** \brief getter
     *
     * \return int - maksymalne zdrowie gracza
     *
     */
    int max_health_get() const;

    /** \brief getter
     *
     * \return const char* - nazwa broni
     *
     */
    const char* weapon_get();

    /** \brief zmniejsza zdrowie gracza
     *
     * \param amount int - ilość obrażeń do zadania
     * \return void
     *
     */
    void takeDamage(int amount);

    /** \brief przywraca zdrowię gracza do maksimum
     *
     * \return void
     *
     */
    void restoreHealth();

    /** \brief sprawdza czy gracz żyje
     *
     * \return bool - true jeśli zdrowie > 0
     *
     */
    bool isAlive() const;

    /** \brief wypisuje statystyki gracza na konsolę
     *
     * \return void
     *
     */
    void printStats() const;

    /** \brief ustawia pozycję gracza
     *
     * \param newX float - nowa pozycja x
     * \param newY float - nowa pozycja y
     * \return void
     *
     */
    void setPosition(float newX, float newY);

    /** \brief przesuwa gracza w stronę myszy
     *
     * \param targetX float - docelowa pozycja x
     * \param targetY float - docelowa pozycja y
     * \return void
     *
     */
    void updatePosition(float targetX, float targetY);

    /** \brief renderuje gracza, broń i wszystkie pociski na ekran
     *
     * \return void
     *
     */
    void render();

    /** \brief getter sprawdzający czy gracz strzelał
     *
     * \return bool - true jeśli są aktywne pociski
     *
     */
    bool is_shooting_get() const;

    /** \brief getter pozycji x pierwszego pocisku
     *
     * \return float - pozycja x pocisku
     *
     */
    float bullet_x_get() const;

    /** \brief getter pozycji y pierwszego pocisku
     *
     * \return float - pozycja y pocisku
     *
     */
    float bullet_y_get() const;

    /** \brief getter liczby aktywnych pocisków
     *
     * \return std::size_t - liczba pocisków w locie
     *
     */
    std::size_t bullets_count_get() const;

    /** \brief getter pozycji x pocisku o określonym indeksie
     *
     * \param idx std::size_t - indeks pocisku
     * \return float - pozycja x pocisku
     *
     */
    float bullet_x_at(std::size_t idx) const;

    /** \brief getter pozycji y pocisku o określonym indeksie
     *
     * \param idx std::size_t - indeks pocisku
     * \return float - pozycja y pocisku
     *
     */
    float bullet_y_at(std::size_t idx) const;

    /** \brief usuwa pocisk o określonym indeksie
     *
     * \param idx std::size_t - indeks pocisku do usunięcia
     * \return void
     *
     */
    void bullet_remove_at(std::size_t idx);

    /** \brief oznacza że pierwszy pocisk trafił
     *
     * \return void
     *
     */
         void bullet_hit();

        /** \brief ruch gracza po ekranie i strzelanie
         *
         * \return void
         *
         */
        void player_move_handler();

        /** \brief sprawdza kolizję ciała gracza z obiektem
         *
         * \param other Object& - obiekt do sprawdzenia
         * \return bool - true jeśli zachodzi kolizja
         *
         */
        bool collision_check_player(Object& other);

        int kill_stat;/**< Liczba zabójstw gracza */
        int death_stat;/**< Liczba śmierci gracza */
        std::string name;/**< Nazwa gracza */

        Object sprite;/**< Tekstura postaci gracza */
        Object gun;/**< Tekstura broni gracza */
        Object bullet;/**< Tekstura pocisku */

        /** \brief getter
         *
         * \return float - pozycja x gracza
         *
         */
        float getX() const;

        /** \brief getter
         *
         * \return float - pozycja y gracza
         *
         */
        float getY() const;

        /** \brief rozpoczyna przeładowanie broni
         *
         * \return void
         *
         */
        void start_reload();

        /** \brief aktualizuje timer przeładowania
         *
         * \return void
         *
         */
        void update_reload();

        /** \brief sprawdza czy gracz przeładowuje broń
         *
         * \return bool - true jeśli trwa przeładowanie
         *
         */
        bool is_reloading() const;

        /** \brief getter ilości amunicji w magazynku
         *
         * \return int - liczba nabojów w bieżącym magazynku
         *
         */
        int ammo_in_mag_get() const;

        /** \brief getter ilości zapasowych magazynków
         *
         * \return int - liczba zapasowych magazynków
         *
         */
        int spare_mags_get() const;

        /** \brief zmniejsza amunicję o 1
         *
         * \return void
         *
         */
        void consume_one_ammo();

        /** \brief resetuje amunicję przy respawnie
         *
         * \return void
         *
         */
        void respawn_reload();

    protected:
        int health;/**< Aktualne zdrowie gracza */
        int max_health;/**< Maksymalne zdrowie gracza */
        int attack;/**< Obrażenia zadawane przez pociski */
        const char* weapon;/**< Nazwa aktualnej broni */

        float x;/**< Pozycja x gracza */
        float y;/**< Pozycja y gracza */
        float r;/**< Promień kolizji gracza */
        float movement_speed;/**< Prędkość ruchu gracza */
        Vec2f speed;/**< Wektor prędkości ruchu */
        double angle;/**< Kąt obrotu (kierunek myszy) */

        bool is_shooting;/**< Flaga czy gracz strzelał */

        /** \struct ActiveBullet
         *
         *  \brief Struktura reprezentująca aktywny pocisk
         */
        struct ActiveBullet
        {
            float x;/**< Pozycja x pocisku */
            float y;/**< Pozycja y pocisku */
            float dx;/**< Prędkość x pocisku */
            float dy;/**< Prędkość y pocisku */
            float angle;/**< Kąt obrotu pocisku */
        };
        std::vector<ActiveBullet> active_bullets;/**< Lista aktywnych pocisków */

        int ammo_in_mag;/**< Liczba nabojów w bieżącym magazynku */
        int spare_mags;/**< Liczba zapasowych magazynków */
        int mag_capacity;/**< Pojemność magazynka */
        bool reloading;/**< Flaga czy trwa przeładowanie */
        int reload_timer_frames;/**< Timer do przeładowania w klatkach */

        int fire_cooldown_frames;/**< Czas oczekiwania między strzałami */
        int fire_rate_frames;/**< Szybkostrzelność broni */



};

#endif
