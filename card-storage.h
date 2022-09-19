#ifndef CARD_STORAGE_H
#define CARD_STORAGE_H

#include "card.h"

#include <string>
#include <vector>
#include <optional>


class CardStorage {
public:
	virtual bool canAccept(const Card & card) const = 0;
    virtual bool acceptCard(const Card & card) = 0;
    virtual const std::optional<Card> topCard() const = 0;
    virtual std::optional<Card> getCard() = 0;
	virtual ~CardStorage() {};
};


class HomeDestination : public CardStorage {
public:
	bool canAccept(const Card & card) const override;
    bool acceptCard(const Card & card) override;
    const std::optional<Card> topCard() const override;
    std::optional<Card> getCard() override;

    friend std::ostream& operator<< (std::ostream& os, const HomeDestination & hd) ;

private:
    std::vector<Card> storage_;
};


class WorkStack : public CardStorage {
public:
	bool canAccept(const Card & card) const override;
    bool acceptCard(const Card & card) override;
    const std::optional<Card> topCard() const override;
    std::optional<Card> getCard() override;

	size_t nbCards() const;

	// avoid canAccept, simply places the card on top
	// useful for game setup
    void forceCard(const Card & card);

    friend std::ostream& operator<< (std::ostream& os, const WorkStack & stack) ;

private:
    std::vector<Card> storage_;
};


class FreeCell : public CardStorage {
public:
	FreeCell & operator=(FreeCell &other) ;

	bool canAccept(const Card & card) const override;
    bool acceptCard(const Card & card) override;
    const std::optional<Card> topCard() const override;
    std::optional<Card> getCard() override;

    Card const * card() const;

private:
    std::optional<Card> cell_;
};

std::ostream& operator<< (std::ostream& os, const FreeCell & fc) ;

#endif
