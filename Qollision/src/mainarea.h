/*
  Copyright (c) 2007 Paolo Capriotti <p.capriotti@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef MAINAREA_H
#define MAINAREA_H

#include <QTimer>
#include <QTime>
#include <QList>
#include <QGraphicsScene>
#ifndef Q_OS_QNX
#include "audioplayer.h"
#endif
#include "animator.h"
#include "message.h"

class Renderer;
class Ball;
class Animation;
class QGraphicsSceneMouseEvent;

struct Collision;

class MainArea : public QGraphicsScene
{
Q_OBJECT
    QTimer m_timer;
    int m_last_time;
    int m_last_game_time;
    QTime m_time;

    /// time interval between two balls being added
    int m_ball_timeout;

    int m_sizeX;
    int m_sizeY;
    Renderer* m_renderer;
    QPixmap m_background;
    Animator m_animator;
    QFont m_msg_font;

    QList<Ball*> m_balls;
    QList<Ball*> m_fading;
    Ball* m_man;

    /// the blue ball is dead
    bool m_death;

    /// the falling animation is over, we're waiting for a new game to start
    bool m_game_over;

    bool m_paused;
    int m_pause_time;
    int m_penalty;

    // on s60 we track player ball movements when player presses button (taps the screen)
#ifdef Q_OS_QNX
    bool m_lmbPressed;
#endif

    QList<MessagePtr> m_welcome_msg;
    QList<MessagePtr> m_pause_msg;

    // no audio yet
#ifndef Q_OS_QNX
    AudioPlayer m_player;
#endif

    double radius() const;
    QPointF randomPoint() const;
    QPointF randomDirection(double val) const;

    Ball* addBall(const QString& id);
    bool collide(const QPointF& a, const QPointF& b,
                double diam, Collision& collision);

    Animation* writeMessage(const QString& text);
    Animation* writeText(const QString& lines, bool fade = true);
    void displayMessages(const QList<KSharedPtr<Message> >& msgs);
    void playSound(int sound);
    void onDeath();
    void setManPosition(const QPointF& p);
    void drawBackground(QPainter*, const QRectF&);
#ifndef Q_OS_QNX
    void updateSounds();
#endif
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
#ifdef Q_OS_QNX
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
#endif
    virtual void focusOutEvent(QFocusEvent*);
public:
    MainArea();
    ~MainArea();
    void start();
public slots:
    void tick();
#ifndef Q_OS_QNX
    void enableSounds(bool enable);
#endif
    void abort();
    void togglePause();
signals:
    void starting();
    void gameOver(int);
    void changeBallNumber(int);
    void changeGameTime(int);
    void changeState(bool);
    void pause(bool);
};

#endif // MAINAREA_H

