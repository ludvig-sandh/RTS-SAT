from dataclasses import dataclass
import pygame
import csv
from math import lcm

@dataclass
class Task:
    arrival: int
    deadline: int
    instanceNumber: int
    taskId: int
    remainingTime: int
    start: int
    end: int
    priority: int
    executionTime: int
    period: int

def read_jobs_from_csv(file_path: str) -> list[Task]:
    tasks = []
    with open(file_path, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            task = Task(
                arrival=int(row['arrival']),
                deadline=int(row['deadline']),
                instanceNumber=int(row['instanceNumber']),
                taskId=int(row['taskId']),
                remainingTime=int(row['remainingTime']),
                start=int(row['start']),
                end=int(row['end']),
                priority=int(row['priority']),
                executionTime=int(row['executionTime']),
                period=int(row['period'])
            )
            tasks.append(task)
    return tasks

# Map value in [s1, e1] to new range [s2, e2]
def mp(value, s1, e1, s2, e2):
    progress = (value - s1) / (e1 - s1)
    mapped = s2 + progress * (e2 - s2)
    return mapped

def draw_arrow(
        surface: pygame.Surface,
        start: pygame.Vector2,
        end: pygame.Vector2,
        color: pygame.Color,
        body_width: int = 2,
        head_width: int = 4,
        head_height: int = 2,
    ):
    """Draw an arrow between start and end with the arrow head at the end.

    Args:
        surface (pygame.Surface): The surface to draw on
        start (pygame.Vector2): Start position
        end (pygame.Vector2): End position
        color (pygame.Color): Color of the arrow
        body_width (int, optional): Defaults to 2.
        head_width (int, optional): Defaults to 4.
        head_height (float, optional): Defaults to 2.
    """
    arrow = start - end
    angle = arrow.angle_to(pygame.Vector2(0, -1))
    body_length = arrow.length() - head_height

    # Create the triangle head around the origin
    head_verts = [
        pygame.Vector2(0, head_height / 2),  # Center
        pygame.Vector2(head_width / 2, -head_height / 2),  # Bottomright
        pygame.Vector2(-head_width / 2, -head_height / 2),  # Bottomleft
    ]
    # Rotate and translate the head into place
    translation = pygame.Vector2(0, arrow.length() - (head_height / 2)).rotate(-angle)
    for i in range(len(head_verts)):
        head_verts[i].rotate_ip(-angle)
        head_verts[i] += translation
        head_verts[i] += start

    pygame.draw.polygon(surface, color, head_verts)

    # Stop weird shapes when the arrow is shorter than arrow head
    if arrow.length() >= head_height:
        # Calculate the body rect, rotate and translate into place
        body_verts = [
            pygame.Vector2(-body_width / 2, body_length / 2),  # Topleft
            pygame.Vector2(body_width / 2, body_length / 2),  # Topright
            pygame.Vector2(body_width / 2, -body_length / 2),  # Bottomright
            pygame.Vector2(-body_width / 2, -body_length / 2),  # Bottomleft
        ]
        translation = pygame.Vector2(0, body_length / 2).rotate(-angle)
        for i in range(len(body_verts)):
            body_verts[i].rotate_ip(-angle)
            body_verts[i] += translation
            body_verts[i] += start

        pygame.draw.polygon(surface, color, body_verts)

def display_schedule(screen, jobs, task_priority_to_id, hyper_period):
    num_tasks = len(task_priority_to_id)

    # pygame stuff
    screen.fill((230, 230, 230))
    fill_color = (0, 0, 0)
    top_border = 50
    bottom_border = HEIGHT - 100
    left_border = 150
    right_border = WIDTH - 50
    w = right_border - left_border
    h = bottom_border - top_border
    task_height = h / (2 * num_tasks + 1)
    task_ys = [top_border + (2 * i + 1) * task_height for i in range(num_tasks)] # Values for top of rectangles
    task_ys = task_ys[::-1]

    makespan = 0
    for job in jobs:
        makespan = max(makespan, job.end)
    makespan = max(makespan, hyper_period)

    for i in range(makespan + 1):
        textsurface = myfont.render(str(i), False, fill_color)
        x = mp(i, 0, makespan, left_border, right_border)
        screen.blit(textsurface, (x - 10, bottom_border + 15))
        pygame.draw.line(screen, fill_color, (x, bottom_border - 5), (x, bottom_border + 5), 1)

    pygame.draw.line(screen, fill_color, (left_border, top_border), (left_border, bottom_border), 2)
    pygame.draw.line(screen, fill_color, (left_border, bottom_border), (right_border, bottom_border), 2)
    for i in range(num_tasks):
        textsurface = myfont.render("Task " + str(task_priority_to_id[num_tasks - i - 1]), False, fill_color)
        screen.blit(textsurface, (20, task_ys[i]))
        draw_arrow(screen, pygame.Vector2(right_border, task_ys[i] + task_height), pygame.Vector2(right_border, task_ys[i] - task_height * 0.25), (150, 150, 150), head_width=task_height / 3, head_height=task_height / 3)

    seen_instance_numbers = {task_id: set() for task_id in task_priority_to_id.values()}
    for job in jobs:
        x_start = mp(job.start, 0, makespan, left_border, right_border)
        x_end = mp(job.end, 0, makespan, left_border, right_border)
        y_top = task_ys[job.priority]
        y_bottom = task_ys[job.priority] + task_height
        x_arrival = mp(job.arrival, 0, makespan, left_border, right_border)
        x_deadline = mp(job.deadline, 0, makespan, left_border, right_border)

        points = [
            (x_start, y_top)
        ]

        if job.instanceNumber in seen_instance_numbers[job.taskId]:
            points.extend([
                (x_start + 10, y_top + task_height / 4),
                (x_start, y_bottom - task_height / 2),
                (x_start + 10, y_bottom - task_height / 4),
            ])

        points.extend([
            (x_start, y_bottom),
            (x_end, y_bottom),
        ])

        if job.remainingTime > 0:
            points.extend([
                (x_end - 10, y_bottom - task_height / 4),
                (x_end, y_bottom - task_height / 2),
                (x_end - 10, y_top + task_height / 4),
            ])

        points.append(
            (x_end, y_top)
        )
        
        rb_for_green = 100 * 0.8 ** job.priority
        color = (rb_for_green, 180 * 0.8 ** job.priority, rb_for_green) if job.end <= job.deadline else (220, 60, 60)
        pygame.draw.polygon(screen, color, points)
        
        draw_arrow(screen, pygame.Vector2(x_arrival, y_top + task_height), pygame.Vector2(x_arrival, y_top - task_height * 0.25), (0, 0, 0), head_width=task_height / 3, head_height=task_height / 3)
        draw_arrow(screen, pygame.Vector2(x_deadline, y_top), pygame.Vector2(x_deadline, y_top + task_height * 1.25), (0, 0, 0), head_width=task_height / 3, head_height=task_height / 3)

        seen_instance_numbers[job.taskId].add(job.instanceNumber)

    pygame.display.flip()

def compute_hyper_period(periods):
    return lcm(*periods)

if __name__ == "__main__":
    file_path = "schedule.csv"  # Replace with your actual file path
    jobs = read_jobs_from_csv(file_path)
    task_priority_to_id = {}
    for job in jobs:
        task_priority_to_id[job.priority] = job.taskId

    WIDTH = 1400
    HEIGHT = 400
    pygame.font.init()
    myfont = pygame.font.SysFont('Comic Sans MS', 30)
    screen = pygame.display.set_mode([WIDTH, HEIGHT])

    periods = list(set(j.period for j in jobs))
    hyper_period = compute_hyper_period(periods)

    while True:
        display_schedule(screen, jobs, task_priority_to_id, hyper_period)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 4:
                    scale *= 0.96
                elif event.button == 5:
                    scale *= 1.04

        keys = pygame.key.get_pressed()  #checking pressed keys
        if keys[pygame.K_d]:
            offset += 0.01
        if keys[pygame.K_a]:
            offset -= 0.01