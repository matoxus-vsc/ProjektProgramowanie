/** \author Mateusz Miozga
 *
 * \file Camera.h
 *
 * \brief Klasa zarządzająca kamerą gry
 */

#pragma once

/** \class Camera
 *
 *  \brief Klasa odpowiadająca za śledzenie i renderowanie widoku kamery
 */
class Camera
{
public:
    /** \brief Konstruktor - inicjacja kamery
     *
     *
     */
    Camera();

    /** \brief aktualizuje pozycję kamery, aby śledzić cel
     *
     * \param target_x float - pozycja x celu do śledzenia
     * \param target_y float - pozycja y celu do śledzenia
     * \param window_w int - szerokość okna
     * \param window_h int - wysokość okna
     * \param map_w int - szerokość mapy
     * \param map_h int - wysokość mapy
     * \return void
     *
     */
    void update(float target_x, float target_y, int window_w, int window_h, int map_w, int map_h);

    /** \brief getter pozycji x widoku kamery
     *
     * \return float - pozycja x górnego lewego rogu widoku
     *
     */
    float view_x_get() const;

    /** \brief getter pozycji y widoku kamery
     *
     * \return float - pozycja y górnego lewego rogu widoku
     *
     */
    float view_y_get() const;

private:
    float view_x;/**< Pozycja x górnego lewego rogu widoku kamery */
    float view_y;/**< Pozycja y górnego lewego rogu widoku kamery */
};
   
