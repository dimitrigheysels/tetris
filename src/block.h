#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <optional>
#include <experimental/random>

#include "global.h"

typedef bool layout_t[BLOCK_LAYOUT_SIZE][BLOCK_LAYOUT_SIZE];

class Tile;

class Block : public std::enable_shared_from_this<Block>
{
public:
    explicit Block(int initial_col) : layout_index_(std::experimental::randint(0, 3)),
                                      position_col_(initial_col),
                                      position_row_(1)
    {
    }

    virtual ~Block() {}

protected:
    int layout_index_;

    // left top of block
    int position_col_;
    int position_row_;

public:
    virtual const layout_t &get_current_layout() const = 0;
    virtual const layout_t &get_layout_after_rotation() const = 0;
    virtual sf::Color get_color() const = 0;

    bool can_down(const std::shared_ptr<Tile> (&tiles_)[MAX_ROWS + 1][MAX_COLS]) const;
    void down();

    bool can_left(const std::shared_ptr<Tile> (&tiles_)[MAX_ROWS + 1][MAX_COLS]) const;
    void left();

    bool can_right(const std::shared_ptr<Tile> (&tiles_)[MAX_ROWS + 1][MAX_COLS]) const;
    void right();

    bool can_rotate(const std::shared_ptr<Tile> (&tiles_)[MAX_ROWS + 1][MAX_COLS]) const;
    void rotate();

    std::optional<int> get_bottom_boundary(int col) const;
    std::optional<int> get_left_boundary(int row) const;
    std::optional<int> get_right_boundary(int row) const;
    int get_top_boundary() const;

    void set_fixed_in_field(const std::shared_ptr<Tile> (&tiles_)[MAX_ROWS + 1][MAX_COLS]);

    int get_position_row() { return position_row_; }
    int get_position_col() { return position_col_; }
};

class I_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{false, true, false, false},  // |P|x| | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false}}, // | |x| | |

                                            {{false, false, false, false},  // |P| | | |
                                             {false, false, false, false},  // | | | | |
                                             {true, true, true, true},      // |x|x|x|x|
                                             {false, false, false, false}}, // | | | | |

                                            {{false, true, false, false},  // |P|x| | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false}}, // | |x| | |

                                            {{false, false, false, false},   // |P| | | |
                                             {false, false, false, false},   // | | | | |
                                             {true, true, true, true},       // |x|x|x|x|
                                             {false, false, false, false}}}; // | | | | |

public:
    explicit I_Block(int initial_col) : Block(initial_col)
    {
    }
    ~I_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};

class S_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{false, false, false, false},  // |P| | | |
                                             {false, true, true, false},    // | |x|x| |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, false, false, false}}, // | | | | |

                                            {{false, false, false, false}, // |P| | | |
                                             {true, false, false, false},  // |x| | | |
                                             {true, true, false, false},   // |x|x| | |
                                             {false, true, false, false}}, // | |x| | |

                                            {{false, false, false, false},  // |P| | | |
                                             {false, true, true, false},    // | |x|x| |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, false, false, false}}, // | | | | |

                                            {{false, false, false, false},  // |P| | | |
                                             {true, false, false, false},   // |x| | | |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, true, false, false}}}; // | |x| | |
public:
    explicit S_Block(int initial_col) : Block(initial_col)
    {
    }

    ~S_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};

class Z_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{false, false, false, false},  // |P| | | |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, true, true, false},    // | |x|x| |
                                             {false, false, false, false}}, // | | | | |

                                            {{false, false, false, false}, // |P| | | |
                                             {false, true, false, false},  // | |x| | |
                                             {true, true, false, false},   // |x|x| | |
                                             {true, false, false, false}}, // |x| | | |

                                            {{false, false, false, false},  // |P| | | |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, true, true, false},    // | |x|x| |
                                             {false, false, false, false}}, // | | | | |

                                            {{false, false, false, false},  // |P| | | |
                                             {false, true, false, false},   // | |x| | |
                                             {true, true, false, false},    // |x|x| | |
                                             {true, false, false, false}}}; // |x| | | |
public:
    explicit Z_Block(int initial_col) : Block(initial_col)
    {
    }

    ~Z_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};

class T_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{false, false, false, false},  // |P| | | |
                                             {false, true, false, false},   // | |x| | |
                                             {true, true, true, false},     // |x|x|x| |
                                             {false, false, false, false}}, // | | | | |

                                            {{false, false, false, false}, // |P| | | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, true, false},   // | |x|x| |
                                             {false, true, false, false}}, // | |x| | |

                                            {{false, false, false, false}, // |P| | | |
                                             {false, false, false, false}, // |x|x|x| |
                                             {true, true, true, false},    // | |x| | |
                                             {false, true, false, false}}, // | | | | |

                                            {{false, false, false, false},  // |P| | | |
                                             {false, true, false, false},   // | |x| | |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, true, false, false}}}; // | |x| | |

public:
    explicit T_Block(int initial_col) : Block(initial_col)
    {
    }

    ~T_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};

class L_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{false, false, false, false}, // |P| | | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, true, false}},  // | |x|x| |

                                            {{false, false, false, false}, // |P| | | |
                                             {false, false, false, false}, // | | | | |
                                             {true, true, true, false},    // |x|x|x| |
                                             {true, false, false, false}}, // |x| | | |

                                            {{false, false, false, false}, // |P| | | |
                                             {true, true, false, false},   // |x|x| | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false}}, // | |x| | |

                                            {{false, false, false, false},   // |P| | | |
                                             {false, false, true, false},    // | | |x| |
                                             {true, true, true, false},      // |x|x|x| |
                                             {false, false, false, false}}}; // | | | | |

public:
    explicit L_Block(int initial_col) : Block(initial_col)
    {
    }

    ~L_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};

class J_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{false, false, false, false}, // |P| | | |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false},  // | |x| | |
                                             {true, true, false, false}},  // |x|x| | |

                                            {{false, false, false, false},  // |P| | | |
                                             {true, false, false, false},   // |x| | | |
                                             {true, true, true, false},     // |x|x|x| |
                                             {false, false, false, false}}, // | | | | |

                                            {{false, false, false, false}, // |P| | | |
                                             {false, true, true, false},   // | |x|x| |
                                             {false, true, false, false},  // | |x| | |
                                             {false, true, false, false}}, // | |x| | |

                                            {{false, false, false, false},  // |P| | | |
                                             {false, false, false, false},  // | | | | |
                                             {true, true, true, false},     // |x|x|x| |
                                             {false, false, true, false}}}; // | | |x| |

public:
    explicit J_Block(int initial_col) : Block(initial_col)
    {
    }

    ~J_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};

class O_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{true, true, false, false},    // |P|x| | |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false}}, // | | | | |

                                            {{true, true, false, false},    // |P|x| | |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false}}, // | | | | |

                                            {{true, true, false, false},    // |P|x| | |
                                             {true, true, false, false},    // |x|x| | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false}}, // | | | | |

                                            {{true, true, false, false},     // |P|x| | |
                                             {true, true, false, false},     // |x|x| | |
                                             {false, false, false, false},   // | | | | |
                                             {false, false, false, false}}}; // | | | | |

public:
    explicit O_Block(int initial_col) : Block(initial_col)
    {
    }

    ~O_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};

class DOT_Block : public Block
{
private:
    static constexpr layout_t LAYOUTS[4] = {{{true, false, false, false},   // |P| | | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false}}, // | | | | |

                                            {{true, false, false, false},   // |P| | | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false}}, // | | | | |

                                            {{true, false, false, false},   // |P| | | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false},  // | | | | |
                                             {false, false, false, false}}, // | | | | |

                                            {{true, false, false, false},    // |P| | | |
                                             {false, false, false, false},   // | | | | |
                                             {false, false, false, false},   // | | | | |
                                             {false, false, false, false}}}; // | | | | |

public:
    explicit DOT_Block() : Block(0)
    {
    }

    ~DOT_Block() {}

    const layout_t &get_current_layout() const override;
    const layout_t &get_layout_after_rotation() const override;
    sf::Color get_color() const override;
};