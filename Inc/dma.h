
#ifndef DMA_H_
#define DMA_H_


#define DMA2_EN					(1U << 22)
#define DMA_CHANNEL_SEL_BIT		(25U)
#define DMA_MSIZE_BIT			(13U)
#define DMA_MINC				(1U << 10)
#define DMA_DIR_BIT				(6U)
#define DMA_TCIE				(1U << 4)
#define DMA_EN					(1U << 0)


enum
{
	CHANNEL_0,
	CHANNEL_1,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4,
	CHANNEL_5,
	CHANNEL_6,
	CHANNEL_7,
};

enum
{
	PERIP_TO_MEM,	// 0: peripheral-to-memory
	MEM_TO_PERIP,	// 1: memory-to-peripheral
	MEM_TO_MEM,		// 2: memory-to-memory
};

void dma_init(void);

#endif /* DMA_H_ */
