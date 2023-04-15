#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(400,120);                                                   // размер окна
    MainSpace   =   new QLabel(this);                                       // фон
    PlayingSong =   new QLabel(this);                                       // проигрываемая песня
    this->setWindowFlags(Qt::FramelessWindowHint);                          // убираем рамку
    this->setAttribute(Qt::WA_TranslucentBackground);                       // делаем окно прозрачным

    MainSpace->resize(this->size());
    MainSpace->show();
    MainSpace->setStyleSheet("border-image: url(:/res/POLOSKA.png);");      // ставим фон

//------------------ Контекстное меню ------------------

    Menu            =   new QMenu(this);
    Action_playlist =   new QAction;
    Action_exit     =   new QAction;

    Action_playlist->setText("Очистить плейлист");
    Action_exit->setText("Выйти");

    Menu->addAction(Action_playlist);
    Menu->addSeparator();
    Menu->addAction(Action_exit);


//------------- Строка отображающая текущую песню ---------------------

    PlayingSong->resize(400,20);
    PlayingSong->move(0,100);
    PlayingSong->setAlignment(Qt::AlignCenter);
    PlayingSong->show();
    PlayingSong->setStyleSheet("background-color: blue; color: yellow");
    PlayingSong->setText("<b>Название трека</b>");

 //--------------------------------------------------------------------

    ReturnButton        =   new QPushButton(this);
    PlayButton          =   new QPushButton(this);
    PauseButton         =   new QPushButton(this);
    StopButton          =   new QPushButton(this);
    NextButton          =   new QPushButton(this);
    PlaylistButton      =   new QPushButton(this);
    PlaylistButtonView  =   new QPushButton(this);
    m_player            =   new QMediaPlayer(this);
    m_playListModel     =   new QStandardItemModel(this);
    m_playlist          =   new QMediaPlaylist(m_player);                        // Инициализируем плейлист
    m_playListModel     =   new QStandardItemModel(this);
    playlistView        =   new QTableView(this);
    m_psldPosition      =   new QSlider(this);

  // ---------------------- Формируем плейлист в таблице -------------------------------------

    playlistView->setModel(m_playListModel);    // Устанавливаем модель данных в TableView
    playlistView->resize(this->width(), this->height()*2);


    // Устанавливаем заголовки таблицы
    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Audio Track")
                                                            << tr("File Path"));

    playlistView->hideColumn(1);    // Скрываем колонку, в которой хранится путь к файлу
    playlistView->verticalHeader()->setVisible(false);                  // Скрываем нумерацию строк
    playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);  // Включаем выделение строк
    playlistView->setSelectionMode(QAbstractItemView::SingleSelection); // Разрешаем выделять только одну строку
    playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Отключаем редактирование

    // Включаем подгонку размера последней видимой колонки к ширине TableView
    playlistView->horizontalHeader()->setStretchLastSection(true);

    playlistView->setWindowFlags(Qt::FramelessWindowHint);                          // убираем рамку
    playlistView->move(10,10);
    playlistView->hide();


    m_player->setPlaylist(m_playlist);                                      // Устанавливаем плейлист в плеер
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);  // Устанавливаем циклический режим проигрывания плейлиста

  //----------------- Прорисовываем кнопки -------------------------------------

    ReturnButton->resize(50,50);
    ReturnButton->move(this->x()+10, this->y()+10);
    ReturnButton->setStyleSheet("border-image: url(:/res/Return_noActive.png)");
    ReturnButton->show();

    PlayButton->resize(50,50);
    PlayButton->move(this->x()+60, this->y()+10);
    PlayButton->setStyleSheet("border-image: url(:/res/Play_noAktive.png)");
    PlayButton->show();

    PauseButton->resize(50,50);
    PauseButton->move(this->x()+110, this->y()+10);
    PauseButton->setStyleSheet("border-image: url(:/res/Pause_noActive.png)");
    PauseButton->show();

    StopButton->resize(50,50);
    StopButton->move(this->x()+160, this->y()+10);
    StopButton->setStyleSheet("border-image: url(:/res/Stop_noActive.png)");
    StopButton->show();

    NextButton->resize(50,50);
    NextButton->move(this->x()+210, this->y()+10);
    NextButton->setStyleSheet("border-image: url(:/res/Next_noActive.png)");
    NextButton->show();

    PlaylistButton->resize(45,45);
    PlaylistButton->move(this->x()+this->width()-60, this->y()+10);
    PlaylistButton->setStyleSheet("border-image: url(:/res/playlist.png)");
    PlaylistButton->show();

    PlaylistButtonView->resize(30,30);
    PlaylistButtonView->move(this->x()+this->width()-120, this->y()+50);
    PlaylistButtonView->setStyleSheet("border-image: url(:/res/down1.png)");
    PlaylistButtonView->show();

    m_psldPosition->resize(400,20);
    m_psldPosition->move(0,80);
    m_psldPosition->setRange(0,0);
    m_psldPosition->setOrientation(Qt::Horizontal);

    connect(PlayButton,         SIGNAL(clicked(bool)),  this,   SLOT(PlayEvent()));
    connect(PauseButton,        SIGNAL(clicked(bool)),  this,   SLOT(PauseEvent()));
    connect(StopButton,         SIGNAL(clicked(bool)),  this,   SLOT(StopEvent()));
    connect(PlaylistButton,     SIGNAL(clicked(bool)),  this,   SLOT(on_btn_add_clicked()));
    connect(NextButton,         SIGNAL(clicked(bool)),  this,   SLOT(NextEvent()));
    connect(ReturnButton,       SIGNAL(clicked(bool)),  this,   SLOT(ReturnEvent()));
    connect(PlaylistButtonView, SIGNAL(clicked(bool)),  this,   SLOT(Playlist_show()));
    connect(Action_exit,        SIGNAL(triggered(bool)),this,   SLOT(Exitaction()));
    connect(Action_playlist,    SIGNAL(triggered(bool)),this,   SLOT(ClearPlaylist()));
    connect(m_psldPosition,     SIGNAL(sliderMoved(int)), SLOT(slotSetMediaPosition(int)));
    connect(m_player,           SIGNAL(positionChanged(qint64)), SLOT(slotSetSliderPosition(qint64)));
    connect(m_player,           SIGNAL(durationChanged(qint64)), SLOT(slotSetDuration(qint64)));

    // при изменении индекса текущего трека в плейлисте, устанавливаем название файла в специальном лейбле
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index)
    {
        PlayingSong->setText("<b>"+m_playListModel->data(m_playListModel->index(index, 0)).toString()+"</b>");
    });

    // При даблклике по треку в таблице устанавливаем трек в плейлисте
    connect(playlistView, &QTableView::doubleClicked, [this](const QModelIndex &index){
        m_playlist->setCurrentIndex(index.row());
        PlayButton->setStyleSheet("border-image: url(:/res/Play_Aktive.png)");
        PauseButton->setStyleSheet("border-image: url(:/res/Pause_noActive.png)");
        StopButton->setStyleSheet("border-image: url(:/res/Stop_noActive.png)");
        status_play = true; status_stop = false; status_pause = false;
        m_player->play();
    });

}

MainWindow::~MainWindow()
{

}

void MainWindow::init()
{

}

void MainWindow::mousePressEvent(QMouseEvent *pe)
{
    if(pe->button()==Qt::RightButton)
    {
        Menu->show();
     } else
    {
        m_ptPosition = pe->pos();
    }

}


//_______________ PLAY _________________________

void MainWindow::mouseMoveEvent(QMouseEvent *pe)
{
    move(pe->globalPos()-m_ptPosition);
}

void MainWindow::PlayEvent()
{
    if(status_play == false)
    {
        status_play = true;
        status_pause = false;
        status_stop = false;
        PlayButton->setStyleSheet("border-image: url(:/res/Play_Aktive.png)");
        PauseButton->setStyleSheet("border-image: url(:/res/Pause_noActive.png)");
        StopButton->setStyleSheet("border-image: url(:/res/Stop_noActive.png)");

        m_player->play();

    }
}

//_________________ PAUSE ___________________________

void MainWindow::PauseEvent()
{
    if((status_pause == false)&&(status_play == true))
    {
        status_play = false;
        status_pause = true;
        status_stop = false;
        PauseButton->setStyleSheet("border-image: url(:/res/Pause_Active.png)");
        PlayButton->setStyleSheet("border-image: url(:/res/Play_noAktive.png)");
        StopButton->setStyleSheet("border-image: url(:/res/Stop_noActive.png)");

        m_player->pause();
    }
}

//__________________ STOP ______________________________

void MainWindow::StopEvent()
{
    if(((status_stop == false)&&(status_play == true))||((status_stop == false)&&(status_pause == true)))
    {
        status_play = false;
        status_pause = false;
        status_stop = true;
        PauseButton->setStyleSheet("border-image: url(:/res/Pause_noActive.png)");
        PlayButton->setStyleSheet("border-image: url(:/res/Play_noAktive.png)");
        StopButton->setStyleSheet("border-image: url(:/res/Stop_Active.png)");

        m_player->stop();
    }
}

//________________________ Далее ___________________________

void MainWindow::NextEvent()
{
    m_playlist->next();
}

//________________________ Назад ___________________________

void MainWindow::ReturnEvent()
{
    m_playlist->previous();
}

//_________________ Добавляем в плейлист ___________________

void MainWindow::on_btn_add_clicked()
{
    // С помощью диалога выбора файлов делаем множественный выбор mp3 файлов
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open files"),
                                                      QString(),
                                                      tr("Audio Files (*.mp3)"));

    // Далее устанавливаем данные по именам и пути к файлам
    // в плейлист и таблицу отображающую плейлист
    foreach (QString filePath, files)
    {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        m_playListModel->appendRow(items);
        m_playlist->addMedia(QUrl("file://" + filePath));
    }

}

// ______________ Показать плейлист _____________________

void MainWindow::Playlist_show()
{
    if(status_playlist == false)
    {
        playlistView->move(0,120);
        this->resize(400,400);
        playlistView->show();
        status_playlist = true;
    } else
    {
        playlistView->move(0,120);
        this->resize(400,120);
        playlistView->hide();
        status_playlist = false;
    }

}

//_______________ Меню выход __________________________

void MainWindow::Exitaction()
{
    exit(0);
}

//_______________ Меню очистить плейлист _______________

void MainWindow::ClearPlaylist()
{
    m_playlist->clear();
    PlayButton->setStyleSheet("border-image: url(:/res/Play_noAktive.png)");
    PauseButton->setStyleSheet("border-image: url(:/res/Pause_noActive.png)");
    StopButton->setStyleSheet("border-image: url(:/res/Stop_noActive.png)");
    status_play = false; status_stop = false; status_pause = false;
    m_player->stop();
    m_playListModel->clear();
    playlistView->reset();

    // Устанавливаем заголовки таблицы
    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Audio Track")
                                                            << tr("File Path"));

    playlistView->hideColumn(1);    // Скрываем колонку, в которой хранится путь к файлу
    playlistView->verticalHeader()->setVisible(false);                  // Скрываем нумерацию строк
    playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);  // Включаем выделение строк
    playlistView->setSelectionMode(QAbstractItemView::SingleSelection); // Разрешаем выделять только одну строку
    playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);   // Отключаем редактирование

    // Включаем подгонку размера последней видимой колонки к ширине TableView
    playlistView->horizontalHeader()->setStretchLastSection(true);

    playlistView->setWindowFlags(Qt::FramelessWindowHint);                          // убираем рамку
    playlistView->move(10,10);
    playlistView->hide();
    playlistView->move(0,120);
    this->resize(400,120);
    playlistView->hide();
    PlayingSong->setText("<b>Название трека</b>");
    status_playlist = false;

}

//----------------------------------------------------------------------------

void MainWindow::slotSetMediaPosition(int n)
{
    m_player->setPosition(n);
}

//----------------------------------------------------------------------------

void MainWindow::slotSetSliderPosition(qint64 n)
{
    m_psldPosition->setValue(n);

}

//----------------------------------------------------------------------------

void MainWindow::slotSetDuration(qint64 n)
{
    m_psldPosition->setRange(0,n);

}


