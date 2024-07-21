![tile.gif](./img/1701396889686-e3636b4d-708a-4090-8c45-65586e7226c3.gif)<br />通过一张原始的静态图，SVD可以生成14帧简单视频，虽说离短剧和视频还很远，但是！表情包走起啊朋友们！
<a name="dfMkw"></a>
# [SVD](https://huggingface.co/stabilityai/stable-video-diffusion-img2vid)
This model was trained to generate 14 frames at resolution 576x1024 given a context frame of the same size. We use the standard image encoder from SD 2.1, but replace the decoder with a temporally-aware deflickering decoder.

<a name="Ke1b0"></a>
## [SVD-XT](https://huggingface.co/stabilityai/stable-video-diffusion-img2vid-xt)
Same architecture as SVD but finetuned for 25 frame generation.

<a name="sYXgz"></a>
# Source
[GitHub - Stability-AI/generative-models: Generative Models by Stability AI](https://github.com/Stability-AI/generative-models)<br />[Stable Video Diffusion: Scaling Latent Video Diffusion Models to Large Datasets — Stability AI](https://stability.ai/research/stable-video-diffusion-scaling-latent-video-diffusion-models-to-large-datasets)<br />[https://static1.squarespace.com/static/6213c340453c3f502425776e/t/655ce779b9d47d342a93c890/1700587395994/stable_video_diffusion.pdf](https://static1.squarespace.com/static/6213c340453c3f502425776e/t/655ce779b9d47d342a93c890/1700587395994/stable_video_diffusion.pdf)
