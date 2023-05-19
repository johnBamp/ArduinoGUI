class Grid {
  private:
    int rows;
    int columns;
    Adafruit_ILI9341* tft;

  public:
    Grid(int rows, int columns, Adafruit_ILI9341* tft): rows(rows), columns(columns), tft(tft) {}

    int getRows() {
      return rows;
    }
    int getColumns() {
      return columns;
    }

    void setRows(int r) {
      rows = r;
    }
    void setColumns(int c) {
      columns = c;
    }
};
