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
    int getRow() const;
    int getCol() const;
    bool getIsHorizontal() const;
    void placeVisually(); 
    void resetVisually();
    
signals:
    void fenceClicked(Fences* f , bool x); 

protected:
    void mousePressEvent(QMouseEvent* event) override;
};