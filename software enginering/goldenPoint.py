from tkinter import *


class GoldenPoint:
    def __init__(self, num_of_students):
        self.N = num_of_students
        self.scores_of_students = [0 for i in range(self.N)]
        self.inputs = []
        self.times = 0
        self.Gs = []

    def input(self, inputs_of_students):
        global goldenMean
        self.inputs.append(inputs_of_students)
        self.Gs.append(avg(self.inputs[len(self.inputs) - 1]) * goldenMean)

    def find_closest_input(self):
        distance = abs(self.inputs[self.times][0] - self.Gs[self.times])
        locations = []
        for i in range(1, self.N):
            if distance > abs(self.inputs[self.times][i] - self.Gs[self.times]):
                distance = abs(self.inputs[self.times][i] - self.Gs[self.times])

        for i in range(0, self.N):
            if abs(self.inputs[self.times][i] - self.Gs[self.times]) == distance:
                locations.append(i)
        return locations

    def find_farthest_input(self):
        distance = abs(self.inputs[self.times][0] - self.Gs[self.times])
        locations = []
        for i in range(1, self.N):
            if distance < abs(self.inputs[self.times][i] - self.Gs[self.times]):
                distance = abs(self.inputs[self.times][i] - self.Gs[self.times])
        for i in range(0, self.N):
            if abs(self.inputs[self.times][i] - self.Gs[self.times]) == distance:
                locations.append(i)
        return locations

    def update_scores(self):
        add_students = self.find_closest_input()
        sub_students = self.find_farthest_input()
        for i in range(len(add_students)):
            self.scores_of_students[add_students[i]] += self.N
        for i in range(len(sub_students)):
            self.scores_of_students[sub_students[i]] -= 2
        self.times += 1

    def get_scores(self):
        return self.scores_of_students

    def get_Gs(self):
        return self.Gs


goldenMean = 0.618
GP = GoldenPoint(1)


def avg(arr):
    return sum(arr) / len(arr)


def init():
    error_N = Label(root, text='人数输入错误')
    if str(N_Entry.get()).isdigit():
        error_N["text"] = ""
        error_N.place(relx=0.8, rely=0.03, relwidth=0.1, relheight=0.1)
        txt.delete(1.0, END)
        global GP
        N_Entry['state'] = 'readonly'
        num_students = int(N_Entry.get())
        GP = GoldenPoint(num_students)
        inputs = str(input_Entry.get()).split()
        inputs = [int(inputs[i]) for i in range(num_students)]
        GP.input(inputs)
        GP.update_scores()
        txt.insert(END, "第" + str(GP.times) + "轮游戏结果：")
        txt.insert(END, GP.get_scores())
        txt.insert(END, "\n")
        input_Entry.delete(0, END)
        init_button.config(state="disabled")
    else:
        error_N.place(relx=0.8, rely=0.03, relwidth=0.1, relheight=0.1)


def run():
    num_students = int(N_Entry.get())
    inputs = str(input_Entry.get()).split()
    error_input = Label(root, text='本轮输入错误')
    if len(inputs) == num_students:
        error_input["text"] = ""
        error_input.place(relx=0.8, rely=0.17, relwidth=0.1, relheight=0.1)
        flag = True
        for i in range(num_students):
            if not str(inputs[i]).isdigit():
                flag = False
                break
        if flag:
            inputs = [int(inputs[i]) for i in range(num_students)]
            GP.input(inputs)
            GP.update_scores()
            txt.insert(END, "第" + str(GP.times))
            txt.insert(END, str(GP.times) + ",游戏结果：")
            txt.insert(END, GP.get_scores())  # 追加显示得分
            txt.insert(END, "\n")
            input_Entry.delete(0, END)  # 清空输入
    else:
        error_input.place(relx=0.8, rely=0.17, relwidth=0.1, relheight=0.1)


def restart():
    N_Entry.delete(0, END)
    input_Entry.delete(0, END)
    txt.delete(1.0, END)
    init_button.config(state="active")
    N_Entry['state'] = 'normal'


root = Tk()
root.geometry('800x500')
root.title('黄金点游戏')

number_label = Label(root, text='请输入参与游戏的人数')
number_label.place(relx=-0.3, relwidth=0.8, relheight=0.1)
N_Entry = Entry(root)
N_Entry.place(relx=0.25, relwidth=0.5, relheight=0.1)

input_label = Label(root, text='请输入本轮数字，以空格分隔')
input_label.place(relx=-0.3, rely=0.15, relwidth=0.8, relheight=0.1)
e = StringVar()
input_Entry = Entry(root, textvariable=e)
input_Entry['show'] = '*'
input_Entry.place(relx=0.25, rely=0.15, relwidth=0.5, relheight=0.1)

init_button = Button(root, text='创建一次游戏', command=init)
init_button.place(relx=0.1, rely=0.4, relwidth=0.2, relheight=0.1)

next_button = Button(root, text='确认本轮输入', command=run)
next_button.place(relx=0.4, rely=0.4, relwidth=0.2, relheight=0.1)

restart_button = Button(root, text='重新开始', command=restart)
restart_button.place(relx=0.7, rely=0.4, relwidth=0.2, relheight=0.1)

# 在窗体垂直自上而下位置60%处起，布局相对窗体高度40%高的文本框
txt = Text(root)
txt.place(rely=0.6, relheight=0.4, relwidth=1)

root.mainloop()


