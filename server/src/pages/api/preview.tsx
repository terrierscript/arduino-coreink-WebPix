import Jimp from 'jimp';
import { render } from '../../lib/render'

export default async (req, res) => {
  const image = await render();
  const buf = await image.getBufferAsync(Jimp.MIME_PNG);
  
  res.setHeader('Content-Type', "image/png")
  res.write(buf)
  res.end()
}