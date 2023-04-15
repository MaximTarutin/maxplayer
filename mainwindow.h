#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QEvent>
#include <QPushButton>
#include <QComboBox>
#include <QMenu>
#include <QAction>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QDir>
#include <QUrl>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QSlider>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPoint  m_ptPosition;                       // позиция виджета на экране
    bool status_play;                           // статус клавиши play
    bool status_pause;                          // статус клавиши пауза
    bool status_stop;                           // статус клавиши стоп
    bool status_playlist;                       // виден или нет плейлист
    QStandardItemModel  *m_playListModel;       // Модель данных плейлиста для отображения

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();

    QLabel          *MainSpace;         // фон виджета
    QMediaPlayer    *m_player;          // плеер
    QMediaPlaylist  *m_playlist;        // плейлист
    QPushButton     *PlayButton;        // кнопка play
    QPushButton     *StopButton;        // кнопка stop
    QPushButton     *PauseButton;       // кнопка pause
    QPushButton     *NextButton;        // кнопка далее
    QPushButton     *ReturnButton;      // кнопка назад
    QPushButton     *PlaylistButton;    // кнопка открыть плейлист
    QPushButton     *PlaylistButtonView;// показать плейлист
    QTableView      *playlistView;      // таблица для отображения плейлиста
    QLabel          *PlayingSong;       // проигрываемая песня
    QMenu           *Menu;              // контекстное меню
    QAction         *Action_exit;       // меню выход
    QAction         *Action_playlist;   // меню очистить плейлист
    QSlider         *m_psldPosition;    // слайдер



public slots:

    void PlayEvent();                                   // нажатие кнопки Play
    void PauseEvent();                                  // нажатие паузы
    void StopEvent();                                   // нажатие стоп
    void on_btn_add_clicked();                          // Слот для обработки добавления треков через диалоговое окно
    void NextEvent();                                   // следующий трек
    void ReturnEvent();                                 // предыдущий трек
    void Playlist_show();                               // показать плейлист
    void Exitaction();                                  // действия в меню выход
    void ClearPlaylist();                               // очистить плейлист
    void slotSetMediaPosition(int n);
    void slotSetSliderPosition(qint64 n);
    void slotSetDuration(qint64 n);

protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent  *pe);
    virtual void contextMenuEvent(QContextMenuEvent* pe)
    {
        Menu->exec(pe->globalPos());
    }

};

#endif // MAINWINDOW_H
