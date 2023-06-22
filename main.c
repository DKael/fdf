/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 17:58:08 by hyungdki          #+#    #+#             */
/*   Updated: 2023/06/22 21:58:58 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void my_mlx_pixel_put(t_fdf *fdf, int x, int y, int color)
{
	char *dst;

	if ((0 <= x && x < fdf->win_size_x) && (0 <= y && y < fdf->win_size_y))
	{
		dst = fdf->img_addr + (y * fdf->size_line + x * (fdf->bpp / 8));
		*(unsigned int *)dst = color;
	}
}

// unsigned int create_trgb(unsigned char t, unsigned char r, unsigned char g, unsigned char b)
// {
// 	return (*(int *)(unsigned char[4]){b, g, r, t});
// }

// unsigned char get_t(int trgb)
// {
// 	return (((unsigned char *)&trgb)[3]);
// }

// unsigned char get_r(int trgb)
// {
// 	return (((unsigned char *)&trgb)[2]);
// }

// unsigned char get_g(int trgb)
// {
// 	return (((unsigned char *)&trgb)[1]);
// }

// unsigned char get_b(int trgb)
// {
// 	return (((unsigned char *)&trgb)[0]);
// }

void fdf_init(t_fdf *fdf)
{
	fdf->mlx_ptr = T_NULL;
	fdf->win_ptr = T_NULL;
	fdf->img_ptr = T_NULL;
	fdf->img_addr = T_NULL;
	fdf->win_size_x = WINDOW_SIZE_X;
	fdf->win_size_y = WINDOW_SIZE_Y;
}

void map_init(t_map *map)
{
	map->map = T_NULL;
	map->row = 0;
	map->col = 0;
	map->fd = -1;
	map->largest_x_2d = (WINDOW_SIZE_X / 2) * -1;
	map->largest_y_2d = (WINDOW_SIZE_Y / 2) * -1;
	map->smallest_x_2d = WINDOW_SIZE_X / 2;
	map->smallest_y_2d = WINDOW_SIZE_Y / 2;
}

void err_msg(const char *msg, int exit_code, t_bool use_perror)
{
	if (msg != T_NULL && use_perror == TRUE)
	{
		ft_fprintf(2, "%s: ", g_program_name);
		perror(msg);
	}
	else if (msg == T_NULL && use_perror == TRUE)
		perror(g_program_name);
	else if (msg != T_NULL && use_perror == FALSE)
		ft_fprintf(2, "%s: %s\n", g_program_name, msg);
	exit(exit_code);
}

void free_2d_array(void **array)
{
	int idx;

	if (array != T_NULL)
	{
		idx = -1;
		while (array[++idx] != T_NULL)
			free(array[idx]);
		free(array);
		array = T_NULL;
	}
}

unsigned int hex_str_to_uint(char *str_color)
{
	int idx;
	unsigned int result;

	idx = 0;
	result = 0;
	while (str_color[idx] != 'x' && str_color[idx] != 'X')
		idx++;
	while (str_color[++idx] != '\0' && str_color[idx] != '\n')
	{
		if ('0' <= str_color[idx] && str_color[idx] <= '9')
			result = result * 16 + (str_color[idx] - '0');
		else if ('a' <= str_color[idx] && str_color[idx] <= 'f')
			result = result * 16 + (str_color[idx] - 'a') + 10;
		else if ('A' <= str_color[idx] && str_color[idx] <= 'F')
			result = result * 16 + (str_color[idx] - 'A') + 10;
		else
			return (0xFFFFFF);
	}
	return (result);
}

void leaks()
{
	system("leaks fdf");
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_printf("Usage : %s <filename>\n", argv[0]);
		return (1);
	}
	atexit(leaks);
	g_program_name = argv[0];
	g_file_name = argv[1];
	fdf();
	return (0);
}

int quit_program(int keycode, t_fdf *fdf)
{
	if (keycode == 53)
	{
		mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
		exit(0);
	}
	return (0);
}
int press_cross_on_window_frame(t_fdf *fdf)
{
	mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
	exit(0);
}

t_bool is_flat(t_point p1, t_point p2, t_point p3, t_point p4)
{
	if ((p2.x - p1.x == p4.x - p3.x && p2.y - p1.y == p4.y - p3.y && p2.z - p1.z == p4.z - p3.z) && (p1.x - p3.x == p2.x - p4.x && p1.y - p3.y == p2.y - p4.y && p1.z - p3.z == p2.z - p4.z))
		return (TRUE);
	else
		return (FALSE);
}

void draw(t_fdf *fdf, t_map *map, int r, int c)
{
	int z00;
	int z01;
	int z10;
	int z11;

	z00 = map->map[r][c].z;
	z01 = map->map[r][c + 1].z;
	z10 = map->map[r + 1][c].z;
	z11 = map->map[r + 1][c + 1].z;
	if ((z00 == z01 && z00 == z10 && z00 != z11) || (z11 == z01 && z11 == z10 && z11 != z00))
		draw_line(fdf, map->map[r][c], map->map[r + 1][c + 1]);
	else if ((z01 == z00 && z01 == z11 && z01 != z10) || (z10 == z00 && z10 == z11 && z10 != z01))
		draw_line(fdf, map->map[r][c + 1], map->map[r + 1][c]);
	else if (z00 < z01 && z00 < z10 && z11 < z01 && z11 < z10)
		draw_line(fdf, map->map[r][c], map->map[r + 1][c + 1]);
	else if (z10 < z11 && z10 < z00 && z01 < z11 && z01 < z00)
		draw_line(fdf, map->map[r][c + 1], map->map[r + 1][c]);
	else if ((z10 < z11 && z10 < z00 && z01 > z11 && z01 > z00) || (z10 > z11 && z10 > z00 && z01 < z11 && z01 < z00))
		draw_line(fdf, map->map[r][c], map->map[r + 1][c + 1]);
	else if ((z00 < z01 && z00 < z10 && z11 > z01 && z11 > z10) || (z00 > z01 && z00 > z10 && z11 < z01 && z11 < z10))
		draw_line(fdf, map->map[r][c + 1], map->map[r + 1][c]);
	else if ((z00 >= z01 && z01 >= z11 && z11 >= z10) || (z00 >= z10 && z10 >= z11 && z11 >= z01) || (z11 >= z10 && z10 >= z00 && z00 >= z01) || (z11 >= z01 && z01 >= z00 && z00 >= z10))
		draw_line(fdf, map->map[r][c + 1], map->map[r + 1][c]);
	else if ((z01 >= z11 && z11 >= z10 && z10 >= z00) || (z01 >= z00 && z00 >= z10 && z10 >= z11) || (z10 >= z00 && z00 >= z01 && z01 >= z11) || (z10 >= z11 && z11 >= z01 && z01 >= z00))
		draw_line(fdf, map->map[r][c], map->map[r + 1][c + 1]);
}

void fdf()
{
	t_map map;
	t_fdf fdf;

	map = map_parsing1();

	// for (int i = 0; i < map.row; i++)
	// {
	// 	for (int j = 0; j < map.col; j++)
	// 	{
	// 		if (map.map[i][j].color.color != (unsigned int)0xFFFFFF)
	// 			ft_printf("%4d,color:%#x", map.map[i][j].z, map.map[i][j].color.color);
	// 		else
	// 			ft_printf("%4d", map.map[i][j].z);
	// 	}
	// 	ft_printf("\n");
	// }

	int r;
	int c;

	r = -1;
	c = -1;
	while (++r < map.row)
	{
		c = -1;
		while (++c < map.col)
		{
			map.map[r][c].x = (map.row / 2) * -1 + c;
			map.map[r][c].y = (map.col / 2) * -1 + r;
			map.map[r][c].x_2d = (map.map[r][c].x - map.map[r][c].y) * ISO_X;
			if (map.map[r][c].x_2d > map.largest_x_2d)
				map.largest_x_2d = map.map[r][c].x_2d;
			else if (map.map[r][c].x_2d < map.smallest_x_2d)
				map.smallest_x_2d = map.map[r][c].x_2d;
			map.map[r][c].y_2d = (map.map[r][c].x + map.map[r][c].y) * ISO_Y - map.map[r][c].z;
			if (map.map[r][c].y_2d > map.largest_y_2d)
				map.largest_y_2d = map.map[r][c].y_2d;
			else if (map.map[r][c].y_2d < map.smallest_y_2d)
				map.smallest_y_2d = map.map[r][c].y_2d;
		}
	}
	map.len_x_2d = map.largest_x_2d - map.smallest_x_2d;
	map.len_y_2d = map.largest_y_2d - map.smallest_y_2d;
	if (map.len_x_2d * ((double)WINDOW_SIZE_Y / WINDOW_SIZE_X) > map.len_y_2d)
		map.basic_len = WINDOW_SIZE_X / map.len_x_2d;
	else
		map.basic_len = WINDOW_SIZE_Y / map.len_y_2d;
	map.midpoint_x_2d = WINDOW_SIZE_X / 2 - (int)((map.largest_x_2d + map.smallest_x_2d) * map.basic_len) / 2;
	map.midpoint_y_2d = WINDOW_SIZE_Y / 2 - (int)((map.largest_y_2d + map.smallest_y_2d) * map.basic_len) / 2;

	fdf_init(&fdf);

	r = -1;
	c = -1;
	while (++r < map.row)
	{
		c = -1;
		while (++c < map.col)
		{
			map.map[r][c].x_2d = map.map[r][c].x_2d * map.basic_len;
			map.map[r][c].rx_2d = (int)(map.map[r][c].x_2d + 0.5) + map.midpoint_x_2d;
			map.map[r][c].y_2d = map.map[r][c].y_2d * map.basic_len;
			map.map[r][c].ry_2d = (int)(map.map[r][c].y_2d + 0.5) + map.midpoint_y_2d;
		}
	}

	fdf.mlx_ptr = mlx_init();
	fdf.win_ptr = mlx_new_window(fdf.mlx_ptr, fdf.win_size_x, fdf.win_size_y, "fdf");
	fdf.img_ptr = mlx_new_image(fdf.mlx_ptr, fdf.win_size_x, fdf.win_size_y);
	fdf.img_addr = mlx_get_data_addr(fdf.img_ptr, &fdf.bpp, &fdf.size_line, &fdf.endian);

	r = -1;
	c = -1;
	while (++r < map.row)
	{
		c = -1;
		while (++c < map.col - 1)
		{
			my_mlx_pixel_put(&fdf, map.map[r][c].rx_2d, map.map[r][c].ry_2d, map.map[r][c].color.color);
			draw_line(&fdf, map.map[r][c], map.map[r][c + 1]);
			if (r < map.row - 1)
				draw_line(&fdf, map.map[r][c], map.map[r + 1][c]);
		}
		if (r < map.row - 1)
			draw_line(&fdf, map.map[r][c], map.map[r + 1][c]);
	}

	r = -1;
	c = -1;
	while (++r < map.row - 1)
	{
		c = -1;
		while (++c < map.col - 1)
		{
			if (!is_flat(map.map[r][c], map.map[r][c + 1], map.map[r + 1][c], map.map[r + 1][c + 1]))
			{
				draw(&fdf, &map, r, c);
			}
		}
	}

	mlx_put_image_to_window(fdf.mlx_ptr, fdf.win_ptr, fdf.img_ptr, 0, 0);

	mlx_key_hook(fdf.win_ptr, quit_program, &fdf);
	mlx_hook(fdf.win_ptr, 17, 0, press_cross_on_window_frame, &fdf);
	mlx_loop(fdf.mlx_ptr);

	free_2d_array((void *)map.map);
}

void draw_line(t_fdf *fdf, t_point p1, t_point p2)
{
	int dx;
	int dy;

	dx = abs(p1.rx_2d - p2.rx_2d);
	dy = abs(p1.ry_2d - p2.ry_2d);
	if (dx == 0 && dy == 0)
		return;
	else if (dx == 0)
		draw_vertical_line(fdf, p1, p2);
	else if (dy == 0)
		draw_horizontal_line(fdf, p1, p2);
	else if (dx == dy)
		draw_line_slope_abs1(fdf, p1, p2);
	else if (dx > dy)
		draw_line_move_x(fdf, p1, p2);
	else if (dx < dy)
		draw_line_move_y(fdf, p1, p2);
}

t_color *calc_color(t_point sp, t_point bp, int np)
{
	double color_dif[4];
	t_color *color_box;
	int idx;
	int idx1;

	color_box = (t_color *)malloc(sizeof(t_color) * np);
	if (color_box == NULL)
	{
		err_msg("malloc error!", 1, FALSE);
	}
	idx = -1;
	while (++idx < 4)
		color_dif[idx] = ((int)bp.color.trgb[idx] - (int)sp.color.trgb[idx]) / (double)np;
	idx1 = -1;
	while (++idx1 < np)
	{
		idx = -1;
		while (++idx < 4)
			color_box[idx1].trgb[idx] = (unsigned char)((double)sp.color.trgb[idx] + color_dif[idx] * (idx1 + 1) + 0.5);
	}
	return color_box;
}

void draw_vertical_line(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point syp;
	t_point byp;
	int y;
	t_color *color_box;
	int idx;

	if (p1.ry_2d < p2.ry_2d)
	{
		syp = p1;
		byp = p2;
	}
	else
	{
		syp = p2;
		byp = p1;
	}
	y = syp.ry_2d;
	color_box = calc_color(syp, byp, byp.ry_2d - syp.ry_2d - 1);
	idx = -1;
	while (++y < byp.ry_2d)
		my_mlx_pixel_put(fdf, p1.rx_2d, y, color_box[++idx].color);
	free(color_box);
}

void draw_horizontal_line(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point sxp;
	t_point bxp;
	int x;
	t_color *color_box;
	int idx;

	if (p1.rx_2d < p2.rx_2d)
	{
		sxp = p1;
		bxp = p2;
	}
	else
	{
		sxp = p2;
		bxp = p1;
	}
	x = sxp.rx_2d;
	color_box = calc_color(sxp, bxp, bxp.rx_2d - sxp.rx_2d - 1);
	idx = -1;
	while (++x < bxp.rx_2d)
		my_mlx_pixel_put(fdf, x, p1.ry_2d, color_box[++idx].color);
	free(color_box);
}

void draw_line_slope_abs1(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point sxp;
	t_point bxp;
	int x;
	int y;
	int move;
	t_color *color_box;
	int idx;

	if (p1.rx_2d < p2.rx_2d)
	{
		sxp = p1;
		bxp = p2;
		y = p1.ry_2d;
	}
	else
	{
		sxp = p2;
		bxp = p1;
		y = p2.ry_2d;
	}
	x = sxp.rx_2d;
	move = -1;
	if ((p2.rx_2d - p1.rx_2d) / (p2.ry_2d - p1.ry_2d) == 1)
		move = 1;
	color_box = calc_color(sxp, bxp, bxp.rx_2d - sxp.rx_2d - 1);
	idx = -1;
	while (++x < bxp.rx_2d)
	{
		y += move;
		my_mlx_pixel_put(fdf, x, y, color_box[++idx].color);
	}
	free(color_box);
}

void draw_line_move_x(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point sxp;
	t_point bxp;
	int x;
	int y;
	t_color *color_box;
	int idx;

	if (p1.rx_2d < p2.rx_2d)
	{
		sxp = p1;
		bxp = p2;
	}
	else
	{
		sxp = p2;
		bxp = p1;
	}
	x = sxp.rx_2d;
	color_box = calc_color(sxp, bxp, bxp.rx_2d - sxp.rx_2d - 1);
	idx = -1;
	while (++x < bxp.rx_2d)
	{
		y = (int)(((double)(p2.ry_2d - p1.ry_2d) / (p2.rx_2d - p1.rx_2d)) * (x - p1.rx_2d) + p1.ry_2d + 0.5);
		my_mlx_pixel_put(fdf, x, y, color_box[++idx].color);
	}
	free(color_box);
}

void draw_line_move_y(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point syp;
	t_point byp;
	int x;
	int y;
	t_color *color_box;
	int idx;

	if (p1.ry_2d < p2.ry_2d)
	{
		syp = p1;
		byp = p2;
	}
	else
	{
		syp = p2;
		byp = p1;
	}
	y = syp.ry_2d;
	color_box = calc_color(syp, byp, byp.ry_2d - syp.ry_2d - 1);
	idx = -1;
	while (++y < byp.ry_2d)
	{
		x = (int)(((double)(p2.rx_2d - p1.rx_2d) / (p2.ry_2d - p1.ry_2d)) * (y - p1.ry_2d) + p1.rx_2d + 0.5);
		my_mlx_pixel_put(fdf, x, y, color_box[++idx].color);
	}
	free(color_box);
}

t_map map_parsing1()
{
	t_map map;
	char *buffer;

	map_init(&map);
	buffer = T_NULL;
	map.fd = open(g_file_name, O_RDONLY);
	if (map.fd == -1)
		err_msg(g_file_name, 1, TRUE);
	while (1)
	{
		buffer = get_next_line(map.fd);
		if (buffer == T_NULL || buffer[0] == '\n')
			break;
		map.row++;
		free(buffer);
	}
	close(map.fd);
	if (map.row == 0)
		err_msg("No line exist!", 1, FALSE);
	map.map = (t_point **)ft_calloc(map.row + 1, sizeof(t_point *));
	if (map.map == T_NULL)
		err_msg("malloc error!", 1, FALSE);
	map_parsing2(&map);
	return (map);
}

void map_parsing2(t_map *map)
{
	int map_row_idx;
	char **split_result;
	char *buffer;

	map->fd = open(g_file_name, O_RDONLY);
	if (map->fd == -1)
	{
		free(map->map);
		err_msg(g_file_name, 1, TRUE);
	}
	map_row_idx = -1;
	while (1)
	{
		buffer = get_next_line(map->fd);
		if (buffer == T_NULL)
			break;
		split_result = ft_split(buffer, ' ');
		free(buffer);
		if (split_result == T_NULL)
		{
			map_parsing_on_error(map, split_result);
			err_msg("malloc error!", 1, FALSE);
		}
		map_parsing3(map, split_result, &map_row_idx);
	}
}

void map_parsing3(t_map *map, char **split_result, int *row_idx)
{
	int split_idx;

	split_idx = 0;
	while (split_result[split_idx] != T_NULL && split_result[split_idx][0] != '\n')
		split_idx++;
	if (map->col != 0 && map->col <= split_idx)
		map_parsing_make_row(map, row_idx, split_idx, split_result);
	else if (map->col != 0 && map->col > split_idx)
	{
		map_parsing_on_error(map, split_result);
		err_msg("Found wrong line length!\n", 1, FALSE);
	}
	else if (map->col == 0)
	{
		if (split_idx != 0)
		{
			map->col = split_idx;
			map_parsing_make_row(map, row_idx, split_idx, split_result);
		}
		else
		{
			map_parsing_on_error(map, split_result);
			err_msg("Empty liine!", 1, FALSE);
		}
	}
}

void map_parsing_on_error(t_map *map, char **split_result)
{
	if (map->fd != -1)
		close(map->fd);
	free_2d_array((void **)map->map);
	free_2d_array((void **)split_result);
}

void map_parsing_make_row(t_map *map, int *row_idx, int split_idx, char **split_result)
{
	map->map[++(*row_idx)] = (t_point *)malloc(sizeof(t_point) * split_idx);
	if (map->map[*row_idx] == T_NULL)
	{
		map_parsing_on_error(map, split_result);
		err_msg("malloc error!", 1, FALSE);
	}
	split_idx = -1;
	while (split_result[++split_idx] != T_NULL && split_result[split_idx][0] != '\n')
	{
		if (ft_strstr(split_result[split_idx], ",") != T_NULL)
			map_parsing_make_row_color(map, row_idx, split_idx, split_result);
		else
		{
			map->map[*row_idx][split_idx].z = ft_atoi_int(split_result[split_idx]);
			if (map->map[*row_idx][split_idx].z == 0 && split_result[split_idx][0] != '0')
			{
				map_parsing_on_error(map, split_result);
				err_msg("Wrong range of integer value!", 1, FALSE);
			}
			map->map[*row_idx][split_idx].color.color = 0xFFFFFF;
		}
	}
	free_2d_array((void **)split_result);
}

void map_parsing_make_row_color(t_map *map, int *row_idx, int split_idx, char **split_result)
{
	char **split_color;

	split_color = ft_split(split_result[split_idx], ',');
	if (split_color == T_NULL)
	{
		map_parsing_on_error(map, split_result);
		err_msg("malloc error!", 1, FALSE);
	}
	map->map[*row_idx][split_idx].z = ft_atoi_int(split_color[0]);
	if (map->map[*row_idx][split_idx].z == 0 && split_color[0][0] != '0')
	{
		map_parsing_on_error(map, split_result);
		free_2d_array((void **)split_color);
		err_msg("Wrong range of integer value!", 1, FALSE);
	}
	map->map[*row_idx][split_idx].color.color = hex_str_to_uint(split_color[1]);
	free_2d_array((void **)split_color);
}