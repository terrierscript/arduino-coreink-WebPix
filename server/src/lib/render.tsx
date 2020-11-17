import Jimp from 'jimp';


export const render = () : Promise<Jimp> => {
  return new Promise((resolve, reject) => {
    new Jimp(200, 200, '#ffffff', async (err, image) => {
      image.scan(100, 100, 50, 50, function (x, y, offset) {
        //@ts-ignore
        this.bitmap.data.writeUInt32BE("#ffffff", offset, );
      });
      image.scan(20, 20, 40, 40, function (x, y, offset) {
        //@ts-ignore
        this.bitmap.data.writeUInt32BE("#ffffff", offset,)
      });

      // image.circle({ radius: 60, x: 60, y: 60 });
      resolve(image)
    })
  })
}