import os
import sys
import imageio
import matplotlib.pyplot as plt
from tqdm import tqdm
from PIL import Image
from pygifsicle import optimize
from joblib import Parallel, delayed


save_path = "benchmarker.out"
res_path = ".tour"
hist_path = ".hist"
citys = {}
dists = {}


METHODS = ['ls', 'sa', 'ga']
ERROR_MESSAGE = "Invalid usage.\n \
    >> python Benchmarker.py METHOD TSP_FILE_NAME\n \
    e.g.:\n>> python Benchmarker.py ls a280\n \
    Acceptable `METHOD` List: {}".format(' '.join(METHODS))


def compress(file_path):
    im = Image.open(file_path)
    im.convert('P')
    im.save(file_path, optimize=True)


def generateGif(gif_name, frames, duration=0.35):
    print(len(frames), "images were written in this GIF.")
    imgs = []
    for image_name in frames:
        imgs.append(imageio.imread(image_name))
    imageio.mimsave(gif_name, imgs, 'GIF', duration=duration)
    optimize(gif_name)


def generateHist(i, citys, hist, x_cords1, y_cords1, _title):
    global frames
    x_cords = [[citys[hist[1][i]][0], citys[hist[1][i+1]][0]]
               for i, node in enumerate(hist[1][:-1])]

    y_cords = [[citys[hist[1][i]][1], citys[hist[1][i+1]][1]]
               for i, node in enumerate(hist[1][:-1])]

    plt.figure(figsize=(12, 9))
    plt.title("Tour Map - {}".format(hist[0]), fontsize="xx-large")
    plt.plot(x_cords, y_cords, color='red')
    plt.scatter(x_cords1, y_cords1, color='blue')
    plt.savefig(_title)
    plt.close()
    compress(_title)


if __name__ == "__main__":
    args = sys.argv
    assert (len(args) == 3 and args[1] in METHODS),  ERROR_MESSAGE
    tsp_file_name = args[2]
    save_path = args[2] + '.' + args[1] + '.' + save_path
    res_path = 'result.' + args[1] + res_path
    hist_path = 'result.' + args[1] + hist_path
    citys_path = tsp_file_name + '.tsp'
    opt_path = tsp_file_name + '.opt.tour'

    try:
        os.mkdir(save_path)
    except OSError:
        os.system("rm -r " + save_path)
        os.mkdir(save_path)

    try:
        os.mkdir(save_path + "/hist.out")
    except OSError:
        files = os.listdir(save_path + "/hist.out")
        for i in files:
            os.remove(save_path + "/hist.out" + i)
        os.removedirs(save_path + "/hist.out")
        os.mkdir(save_path + "/hist.out")

    with open(citys_path, "r") as f:
        for i in f.readlines()[6:-1]:
            line = [float(j) for j in i[:-1].split()]
            citys[line[0]] = (line[1], line[2])

    # result.**.tour
    with open(res_path, "r") as f:
        res_data = f.readlines()[0]

    with open(hist_path, "r") as f:
        hist_data = f.readlines()

    path = [int(i) for i in res_data.split()[:-1]]
    dist = [float(i) for i in hist_data[1][:-1].split()]
    path_hist = [(float(hist_data[i][:-1]), [int(i) for i in hist_data[i+1][:-1].split()])
                 for i in range(5, len(hist_data)-1, 2)]
    if path_hist[0][0] - path_hist[1][0] > 1e5:
        path_hist = path_hist[1:]
    with open(opt_path, "r") as f:
        data = f.readlines()[:-1]
        while not data[0][:-1].isdigit():
            data = data[1:]
        path_opt = [int(i[:-1]) for i in data]

    x_cords1 = [[citys[path_opt[i]][0], citys[path_opt[i+1]][0]]
                for i, node in enumerate(path_opt[:-1])]
    y_cords1 = [[citys[path_opt[i]][1], citys[path_opt[i+1]][1]]
                for i, node in enumerate(path_opt[:-1])]

    x_cords2 = [[citys[path[i]][0], citys[path[i+1]][0]]
                for i, node in enumerate(path[:-1])]
    y_cords2 = [[citys[path[i]][1], citys[path[i+1]][1]]
                for i, node in enumerate(path[:-1])]

    plt.figure(figsize=(20, 15))
    plt.subplot(221)
    plt.title(tsp_file_name + " - Distance - Loop", fontsize="xx-large")
    plt.plot(range(len(dist)), dist)
    plt.tight_layout()
    # plt.savefig(save_path + '/' + args[1]+'_distance2loop.png')
    # plt.close()
    # compress(save_path + '/' + args[1]+'_distance2loop.png')

    plt.subplot(223)
    plt.title(tsp_file_name + " - Optimal Tour Map", fontsize="xx-large")
    plt.plot(x_cords1, y_cords1, color='red')
    plt.scatter(x_cords1, y_cords1, color='blue')
    plt.tight_layout()
    # plt.savefig(save_path + '/' + args[1]+'_optimal_tour_map.png')
    # plt.close()
    # compress(save_path + '/' + args[1]+'_optimal_tour_map.png')

    plt.subplot(224)
    plt.title(tsp_file_name + " - Result Tour Map - " +
              str(path_hist[-1][0]), fontsize="xx-large")
    plt.plot(x_cords2, y_cords2, color='green')
    plt.scatter(x_cords1, y_cords1, color='blue')
    plt.tight_layout()
    # plt.savefig(save_path + '/' + args[1]+'_result_tour_map.png')
    # plt.close()
    # compress(save_path + '/' + args[1]+'_result_tour_map.png')

    plt.subplot(222)
    plt.title("Overlapped Tour Map", fontsize="xx-large")
    plt.plot(x_cords1, y_cords1, color='red')
    plt.plot(x_cords2, y_cords2, color='green')
    plt.scatter(x_cords1, y_cords1, color='blue')
    plt.tight_layout()
    # plt.savefig(save_path + '/' + args[1]+'_overlapped_tour_map.png')
    # plt.close()
    # compress(save_path + '/' + args[1]+'_overlapped_tour_map.png')

    plt.savefig(save_path + '/' + args[1]+'.png')
    plt.close()
    compress(save_path + '/' + args[1]+'.png')

    hist_len = len(path_hist)
    if hist_len > 200:
        history = path_hist[:-50:int((hist_len - 50) / 150)] + path_hist[-50:]
        hist_len = len(history)
    else:
        history = path_hist

    print(">> Generating {} histories...".format(hist_len))
    history = enumerate(history)

    frames = [save_path + "/hist.out/" +
              args[1]+'_hist_{}.png'.format(i) for i in range(hist_len)]

    Parallel(n_jobs=-2)(delayed(generateHist)
                        (i, citys, hist, x_cords1, y_cords1, save_path + "/hist.out/" +
                         args[1]+'_hist_{}.png'.format(i)) for i, hist in tqdm(history))

    # for i, hist in tqdm(history):
    #     x_cords = [[citys[hist[1][i]][0], citys[hist[1][i+1]][0]]
    #                for i, node in enumerate(hist[1][:-1])]
    #     y_cords = [[citys[hist[1][i]][1], citys[hist[1][i+1]][1]]
    #                for i, node in enumerate(hist[1][:-1])]

    #     plt.figure(figsize=(12, 9))
    #     plt.title("Tour Map - {}".format(hist[0]), fontsize="xx-large")
    #     plt.plot(x_cords, y_cords, color='red')
    #     plt.scatter(x_cords1, y_cords1, color='blue')
    #     frames.append(save_path + "/hist.out/" +
    #                   args[1]+'_hist_{}.png'.format(i))
    #     plt.savefig(save_path + "/hist.out/" +
    #                 args[1]+'_hist_{}.png'.format(i))
    #     plt.close()
    #     compress(save_path + "/hist.out/" +
    #              args[1]+'_hist_{}.png'.format(i))

    generateGif(save_path + "/hist.out/hist.gif", frames)
