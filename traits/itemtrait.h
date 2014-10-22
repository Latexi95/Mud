#ifndef ITEMTRAIT_H
#define ITEMTRAIT_H

class ItemTrait {
	public:
		ItemTrait();
		virtual ~ItemTrait();
		virtual ItemTrait *clone() const = 0;
		const char *traitName() const = 0;
};

#endif // ITEMTRAIT_H
