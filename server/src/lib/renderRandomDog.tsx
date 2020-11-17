import Jimp from 'jimp';
import axios from "axios"
import floydSteinberg from "floyd-steinberg"

const cropSize = (image) => {
  const {width , height} =image.bitmap
  return {
    x: Math.max(width / 2 - 200, 0),
    y: Math.max(height / 2 - 200, 0),
  }
}
export const render = async (): Promise<Jimp> => {
  const { data } = await axios.get("https://dog.ceo/api/breeds/image/random");
  console.log(data)

  const imgUrl = data.message
  // const imgUrl = "https://images.dog.ceo/breeds/kuvasz/n02104029_31.jpg"
  return new Promise((resolve, reject) => {
    Jimp.read(imgUrl)
      .then(image => {
        const {width , height} =image.bitmap
        const resize = width < height
        ? [200, Jimp.AUTO] : [Jimp.AUTO, 200]
        // const resize = width > height
        //   ? [200, Jimp.AUTO] : [Jimp.AUTO, 200]
        image.resize(resize[0], resize[1])
        image.rotate(270)
        const crop = cropSize(image)
        image.crop(crop.x, crop.y, 200,200)
          .grayscale()
        image.bitmap = floydSteinberg(image.bitmap)
      
        // image.resize(200,200)
        resolve(image)
      })
  })
}