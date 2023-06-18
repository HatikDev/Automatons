from collections import deque
from PIL import Image, ImageDraw


class Node:
    def __init__(self, id, parent, left, right, value):
        self._id = id
        self._parent = parent
        self._left = left
        self._right = right
        self._value = value

    def parent(self):
        return self._parent

    def left(self):
        return self._left

    def right(self):
        return self._right

    def value(self):
        return self._value


def read_nodes(filename):
    file = open(filename)
    nodes = {}
    params = []
    is_first_node = True
    for line in file.readlines():
        params = line.split()
        for param in params:
            key, value = map(str, param.split(":"))
            # match key:
            #     case "id": node_id = value
            #     case "left": node_left = value
            #     case "right": node_right = value
            #     case "value": node_value = value
            if key == "id":
                node_id = value
            if key == "parent":
                node_parent = value
            if key == "left":
                node_left = value
            if key == "right":
                node_right = value
            if key == "value":
                node_value = value
        if is_first_node:
            nodes["root"] = node_id
        nodes[node_id] = Node(node_id, node_parent, node_left, node_right, node_value)
        is_first_node = False
    file.close()
    return nodes


def print_tree(nodes, draw_object):
    level = 0
    old_level = -1
    offset_in_level = 1
    current_nodes = deque([(nodes["root"], 0)])
    while len(current_nodes):
        current_node_id, level = current_nodes.popleft()
        if level == old_level:
            offset_in_level += 1
        else:
            offset_in_level = 1
        value = nodes[current_node_id].value()

        x0 = WIDTH / (2 ** level + 1) * offset_in_level
        y0 = 30 + level * 100
        draw_object.ellipse([x0 - 25, y0 - 25, x0 + 25, y0 + 25], fill=(44, 27, 133, 0))
        draw_object.text((x0 - 20, y0 - 10), value, (150, 150, 150))

        left = nodes[current_node_id].left()
        right = nodes[current_node_id].right()
        if int(left, 16) > 0:
            current_nodes.extend([(left, level + 1)])
        if int(right, 16) > 0:
            current_nodes.extend([(right, level + 1)])
        old_level = level


WIDTH = 1440
HEIGHT = 900


img = Image.new(mode="RGB", size=(WIDTH, HEIGHT), color=(255, 255, 255))
draw_object = ImageDraw.Draw(img)
print_tree(read_nodes("ast.txt"), draw_object)
img.show()
