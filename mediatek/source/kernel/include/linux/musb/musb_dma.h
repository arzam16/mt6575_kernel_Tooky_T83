

#ifndef __MUSB_DMA_H__
#define __MUSB_DMA_H__

struct musb_hw_ep;


#define	DMA_ADDR_INVALID	(~(dma_addr_t)0)

enum dma_channel_status {
	/* unallocated */
	MUSB_DMA_STATUS_UNKNOWN,
	/* allocated ... but not busy, no errors */
	MUSB_DMA_STATUS_FREE,
	/* busy ... transactions are active */
	MUSB_DMA_STATUS_BUSY,
	/* transaction(s) aborted due to ... dma or memory bus error */
	MUSB_DMA_STATUS_BUS_ABORT,
	/* transaction(s) aborted due to ... core error or USB fault */
	MUSB_DMA_STATUS_CORE_ABORT
};

struct dma_controller;

struct dma_channel {
	void			*private_data;
	/* FIXME not void* private_data, but a dma_controller * */
	size_t			max_len;
	size_t			actual_len;
	enum dma_channel_status	status;
	bool			desired_mode;
};

static inline enum dma_channel_status
dma_channel_status(struct dma_channel *c)
{
	return (c) ? c->status : MUSB_DMA_STATUS_UNKNOWN;
}

struct dma_controller {
	int			(*start)(struct dma_controller *);
	int			(*stop)(struct dma_controller *);
	struct dma_channel	*(*channel_alloc)(struct dma_controller *,
					struct musb_hw_ep *, u8 is_tx);
	void			(*channel_release)(struct dma_channel *);
	int			(*channel_program)(struct dma_channel *channel,
							u16 maxpacket, u8 mode,
							dma_addr_t dma_addr,
							u32 length);
	int			(*channel_abort)(struct dma_channel *);
	int			(*is_compatible)(struct dma_channel *channel,
							u16 maxpacket,
							void *buf, u32 length);
};

/* called after channel_program(), may indicate a fault */
extern void musb_dma_completion(struct musb *musb, u8 epnum, u8 transmit);


extern struct dma_controller *__init
dma_controller_create(struct musb *, void __iomem *);

extern void dma_controller_destroy(struct dma_controller *);

#endif	/* __MUSB_DMA_H__ */
