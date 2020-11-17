import Jimp from 'jimp';
import split from "just-split"
import { imageToPixhex } from '../../../lib/imageToPixhex'
import { render } from '../../../lib/renderRandomDog'

export default async (req, res) => {
  const image = await render();
  // const data = image.bitmap.data
  const pixhex = imageToPixhex(image)
  
  // console.log(pixbyte)
  // res.write()
  res.json(pixhex.join(""))
  res.end()

}