/** \author Mateusz Miozga
 *
 * \file Bot.h
 *
 * \brief Klasa reprezentująca sztuczną inteligencję bota w grze
 */

#ifndef BOT_H
#define BOT_H

#include "Player.h"
#include <vector>

/** \class Bot
 *
 *  \brief Klasa bota dziedzicząca z Player, zawierająca logikę AI
 */
class Bot : public Player
{
public:
    /** \brief Konstruktor - inicjacja bota
     *
     * \param health int - liczba punktów zdrowia bota
     * \param attack int - obrażenia zadawane przez pociski bota
     *
     */
    Bot(int health, int attack);

    /** \brief Destruktor
     *
     *
     */
    ~Bot();

    /** \brief inicjalizuje bota z docelową pozycją
     *
     * \param goalX float - docelowa pozycja x
     * \param goalY float - docelowa pozycja y
     * \return void
     *
     */
    void init(float goalX, float goalY);

    /** \brief ustawia indeks bota dla spawn pointów
     *
     * \param idx int - indeks bota (0-7)
     * \return void
     *
     */
    void set_bot_index(int idx);

    /** \brief aktualizuje AI bota, zarządza ruchem i strzelaniem
     *
     * \param enemy Player& - gracz (wróg bota)
     * \return void
     *
     */
    void updateAI(Player& enemy);

    /** \brief renderuje bota, jego broń i pociski na ekran
     *
     * \return void
     *
     */
    void render();

    /** \brief respawnuje bota w losowym spawn punkcie
     *
     * \return void
     *
     */
    void respawn();

    /** \brief getter sprawdzający czy bot strzelał
     *
     * \return bool - true jeśli są aktywne pociski
     *
     */
    bool is_shooting_get() const;

    /** \brief getter pozycji x pierwszego pocisku bota
     *
     * \return float - pozycja x pocisku
     *
     */
    float bullet_x_get() const;

    /** \brief getter pozycji y pierwszego pocisku bota
     *
     * \return float - pozycja y pocisku
     *
     */
    float bullet_y_get() const;

    /** \brief getter liczby aktywnych pocisków bota
     *
     * \return std::size_t - liczba pocisków w locie
     *
     */
    std::size_t bullets_count_get() const;

    /** \brief getter pozycji x pocisku bota o określonym indeksie
     *
     * \param idx std::size_t - indeks pocisku
     * \return float - pozycja x pocisku
     *
     */
    float bullet_x_at(std::size_t idx) const;

    /** \brief getter pozycji y pocisku bota o określonym indeksie
     *
     * \param idx std::size_t - indeks pocisku
     * \return float - pozycja y pocisku
     *
     */
    float bullet_y_at(std::size_t idx) const;

    /** \brief oznacza że pierwszy pocisk bota trafił
     *
     * \return void
     *
     */
    void bullet_hit();

    /** \brief usuwa pocisk bota o określonym indeksie
     *
     * \param idx std::size_t - indeks pocisku do usunięcia
     * \return void
     *
     */
    void bullet_remove_at(std::size_t idx);

    /** \brief getter czasu oczekiwania na następny strzał
     *
     * \return int - liczba klatek do następnego strzału
     *
     */
    int fire_cooldown_get() const;

    /** \brief próbuje wystrzelić do celu w określonej odległości
     *
     * \param targetCenterX float - pozycja x celu
     * \param targetCenterY float - pozycja y celu
     * \param distance float - odległość do celu
     * \return bool - true jeśli bot strzelił
     *
     */
    bool try_shoot_at(float targetCenterX, float targetCenterY, float distance);

    /** \brief ustawia pozycję pocisku bota
     *
     * \param x float - pozycja x
     * \param y float - pozycja y
     * \return void
     *
     */
    void bullet_set_position(float x, float y);

    /** \brief ustawia prędkość pocisku bota 
     *
     * \param dx float - prędkość x
     * \param dy float - prędkość y
     * \return void
     *
     */
    void bullet_set_velocity(float dx, float dy);

    /** \brief rozpoczyna strzelanie bota 
     *
     * \return void
     *
     */
    void bullet_start_shooting();

private:
    float goal_x;/**< Docelowa pozycja x bota */
    float goal_y;/**< Docelowa pozycja y bota */
    unsigned int goal_seed;/**< Unikalny seed dla losowych celów każdego bota */
    int bot_index;/**< Indeks bota - do wyboru spawn pointu (0-7) */

    float bot_speed;/**< Prędkość ruchu bota */
    float bot_angle;/**< Kąt obrotu bota (kierunek) */

    /** \struct ActiveBullet
     *
     *  \brief Struktura reprezentująca aktywny pocisk bota
     */
    struct ActiveBullet
    {
        float x;/**< Pozycja x pocisku */
        float y;/**< Pozycja y pocisku */
        float dx;/**< Prędkość x pocisku */
        float dy;/**< Prędkość y pocisku */
        float angle;/**< Kąt obrotu pocisku */
    };
    std::vector<ActiveBullet> active_bullets;/**< Lista aktywnych pocisków bota */
    float bullet_speed;/**< Prędkość pocisków bota */

    int fire_cooldown;/**< Czas oczekiwania między strzałami */
    int hide_timer;/**< Timer dla ukrywania się */
    int patrol_timer;/**< Timer dla patrolu */
    int patrol_dir;/**< Kierunek patrolu (1 lub -1) */
    int goal_change_timer;/**< Timer do zmiany celu patrolu */

    float detection_range;/**< Zasięg widzenia bota */

    int bot_personality;/**< Typ osobowości: 0 = agresywny, 1 = taktyczny, 2 = defensywny */
    int movement_timer;/**< Timer dla taktycznego ruchu (strafe) */
    float strafe_dir_x;/**< Kierunek strafowania x */
    float strafe_dir_y;/**< Kierunek strafowania y */

    int burst_shots_left;/**< Pozostałe strzały w serii */
    int burst_pause_timer;/**< Czas przerwy między seriami */

    /** \brief przesuwa bota z ograniczeniami mapy
     *
     * \param dx float - przesunięcie x
     * \param dy float - przesunięcie y
     * \return void
     *
     */
    void moveWithBounds(float dx, float dy);

    /** \brief zmienia cel patrolu na losowy punkt mapy
     *
     * \return void
     *
     */
    void change_goal();

    /** \brief strzelają do określonego celu
     *
     * \param targetCenterX float - pozycja x celu
     * \param targetCenterY float - pozycja y celu
     * \return void
     *
     */
    void shoot_at(float targetCenterX, float targetCenterY);


};

#endif
