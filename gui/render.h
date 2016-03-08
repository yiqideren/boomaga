/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 *
 * Copyright: 2012-2013 Boomaga team https://github.com/Boomaga
 * Authors:
 *   Alexander Sokoloff <sokoloff.a@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */


#ifndef RENDER_H
#define RENDER_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QList>

namespace poppler
{
    class document;
}

class RenderWorker: public QObject
{
    Q_OBJECT
public:
    explicit RenderWorker(const QString &fileName, int resolution);
    virtual ~RenderWorker();

    bool isBussy() const { return mBussy; }
    QThread *thread() { return &mThread; }

public slots:
    void render(int sheetNum);

signals:
    void imageReady(QImage image, int sheetNum);

private:
    int mSheetNum;
    int mResolution;
    bool mBussy;
    QThread mThread;
    poppler::document *mPopplerDoc;
};


class Render : public QObject
{
    Q_OBJECT
public:
    explicit Render(double resolution, int threadCount = 8, QObject *parent = 0);
    virtual ~Render();


public slots:
    void render(int sheetNum);
    void cancel(int sheetNum);
    void setFileName(const QString &fileName);

signals:
    void imageReady(QImage image, int sheetNum) const;

private slots:
    void workerFinished();

private:
    QVector<RenderWorker*> mWorkers;
    int mResolution;
    int mThreadCount;
    QList<int> mQueue;
};

#endif // RENDER_H