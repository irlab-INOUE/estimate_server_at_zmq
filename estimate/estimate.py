# get3Durgから自己位置と3Durgデータを受信したら，推論結果をoccMap上に反映し，推論結果をexitに送信する．

import numpy as np
import math
import random
import zmq
import time
import cv2
import yaml
import struct

def start_server():
    # mapInfoの読み込み
    with open('mapInfo.yaml', 'r') as file:
        mapInfo = yaml.safe_load(file)
        csize = mapInfo['csize']
        margin = mapInfo['margin']
        originX = mapInfo['originX'] + margin
        originY = mapInfo['originY'] + margin

    # 占有孔子地図を表示
    map_img = cv2.imread('occMap.png')
    cv2.circle(map_img, (originX, originY), 2, (0, 255, 0), thickness=1)
    cv2.circle(map_img, (originX, originY), 4, (0, 255, 0), thickness=1)
    cv2.imshow('image', map_img)
    cv2.waitKey(1000)
    cv2.destroyAllWindows()


    # ZMQの設定
    ctx1 = zmq.Context()
    sock1 = ctx1.socket(zmq.REP)
    sock1.bind("tcp://*:5556")      # getdisrance --> estimate

    ctx2 = zmq.Context()
    sock2 = ctx2.socket(zmq.REQ)
    sock2.bind("tcp://*:5557")      # estimate --> exit
    
    print("Server startup.")


    while True:
        # 自己位置の受信
        rcv1x = sock1.recv()
        sock1.send_string("Reply: %s" % rcv1x)
        locX = struct.unpack('d', rcv1x)
        locX = int(locX[0])

        rcv1y = sock1.recv()
        sock1.send_string("Reply: %s" % rcv1y)
        locY = struct.unpack('d', rcv1y)
        locY = int(locY[0])

        rcv1a = sock1.recv()
        sock1.send_string("Reply: %s" % rcv1a)
        angle = struct.unpack('d', rcv1a)
        angle = int(angle[0])
        angle = math.radians(angle)

        # 座標->ピクセル変換
        x_on_img = originX + int(locX / csize)
        y_on_img = originY - int(locY / csize)
        print("x : %d, y : %d" % (x_on_img, y_on_img))
        cv2.circle(map_img, (x_on_img, y_on_img), 3, (0, 100, 255), thickness=-1)
        cv2.imshow('image', map_img)
        cv2.waitKey(1000)
        cv2.destroyAllWindows()


        # LiDARデータの受信（ダミー結果）
        Nr = 1
        Nt = 36
        result_list = np.zeros((Nr, Nt))
        for i in range(Nr):
            for j in range(Nt):
                r = random.randint(0, 100)
                if r >= 15:
                    result_list[i, j] = True
                if r == 0:
                    result_list[i, j] = False


        # 回転前の元画像
        LUT = np.load('lookup_table.npy')
        height = LUT.shape[0]
        width = LUT.shape[1]

        origin_img = np.full((height, width, 3), 255, dtype=np.uint8)
        # v:縦，u:横
        for v in range(height):
            for u in range(width):
                for i in range(Nr):
                    for j in range(Nt):
                        if LUT[v, u, 4] == i and LUT[v, u, 5] == j:
                            val = result_list[i, j]
                            if val == True:
                                color = [255, 0, 0]
                            if val == False:
                                color = [0, 0, 255]
                            origin_img[v, u] = color


        # 回転した画像
        rot_img = np.full((height, width, 3), 255, dtype=np.uint8)
        for v in range(height):
            for u in range(width):
                x_ = -width / 2 + u
                y_ = height / 2 - v
                x = x_ * math.cos(angle) + y_ * math.sin(angle)
                y = -x_ * math.sin(angle) + y_ * math.cos(angle)
                x = int(x + width / 2)
                y = -int(y - height / 2)
                if 0 <= x < width and 0 <= y < height:
                    color = origin_img[y, x]
                    rot_img[v, u] = color

        #merge_img = np.hstack((origin_img, rot_img))
        #cv2.imshow("compare", merge_img)
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()


        # mapに当てはめ
        for v in range(height):
            for u in range(width):
                color = rot_img[v, u]
                if np.any(color != 255):
                    v_ = u
                    u_ = height - v
                    v_ = v_ - width / 2
                    u_ = u_ - height / 2
                    v_ = v_ + originY
                    u_ = u_ + originX
                    v_ = v_ - locY / csize
                    u_ = u_ + locX / csize
                    map_img[int(v_), int(u_)] = color

        cv2.imshow("OccMap", map_img)
        cv2.waitKey(1000)
        cv2.destroyAllWindows()


        # error処理
        for i in range(Nr):
            for j in range(Nt):
                num = int(i * Nt + j)
                sock2.send(str(num).encode())
                rcv2n = sock2.recv()
                result = int(result_list[i, j])
                sock2.send(str(result).encode())
                rcv2r = sock2.recv()
                #message = struct.unpack('i', rcv_msg)
                #message = int(message[0])
                #print(message)
    sock1.close()
    ctx1.destroy()



if __name__ == "__main__":
    start_server()

