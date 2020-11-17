import Jimp from 'jimp';
import { render } from '../../lib/render'
import split from "just-split"

export default async (req, res) => {
  const image = await render();
  const data = image.bitmap.data
  let binmap = []
  image.scan(0, 0, image.bitmap.width, image.bitmap.height, (x, y, idx) => {
    // console.log(x,y,data[idx + 0])
    
    const bb = (data[idx + 0] + data[idx + 1] + data[idx + 2]) / 3
    const bin = (bb < 128) ? 0: 1
    binmap.push( bin)
    return bin
  })
  // console.log(JSON.stringify(binmap))
  const chunks = split(binmap, 8)
  const pixbyte = chunks.map(c => {
    return parseInt(c.join(""),2).toString(16).padStart(2, "0")
  })

  // console.log(pixbyte)
  // res.write()
  res.json(pixbyte.join(""))
  res.end()

}