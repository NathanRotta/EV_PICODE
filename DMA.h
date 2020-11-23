

#define DMA_CHANNEL0_BASE  0x7000

class DMA{
	public:
	DMA();
	~DMA();
	private:
	bcm2711_peripheral DMAPeri;
}
