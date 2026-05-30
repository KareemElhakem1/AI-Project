#pragma once
#include <QLabel>
#include <QMouseEvent>

class Fences : public QLabel {
    Q_OBJECT
private:
    int row, col;
    bool isHorizontal;

public:
    Fences(int r, int c, bool horizontal, QWidget* parent = nullptr);
    
    // Add these three Getters!
    int getRow() const;
    int getCol() const;
    bool getIsHorizontal() const;

    void placeVisually(); 

signals:
    void fenceClicked(Fences* f); 

protected:
    void mousePressEvent(QMouseEvent* event) override;
};