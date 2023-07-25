/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw3_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyungdki <hyungdki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 19:37:34 by hyungdki          #+#    #+#             */
/*   Updated: 2023/07/25 17:51:41 by hyungdki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf_bonus.h"

static void	draw_vertical_line(t_fdf *fdf, t_point p1, t_point p2);
static void	draw_vertical_line2(t_fdf *fdf, t_point syp, t_point byp);
static void	draw_vertical_line3(t_fdf *fdf,
				t_point syp, t_point byp, t_color *color_box);
extern void	draw_horizontal_line(t_fdf *fdf, t_point p1, t_point p2);
extern void	draw_line_slope_abs(t_fdf *fdf, t_point p1, t_point p2);
extern void	draw_line_move_x(t_fdf *fdf, t_point p1, t_point p2);
extern void	draw_line_move_y(t_fdf *fdf, t_point p1, t_point p2);

void	draw_line(t_fdf *fdf, t_point p1, t_point p2)
{
	int	dx;
	int	dy;

	dx = abs(p1.rx2d - p2.rx2d);
	dy = abs(p1.ry2d - p2.ry2d);
	if (dx == 0 && dy == 0)
		return ;
	else if (dx == 0)
		draw_vertical_line(fdf, p1, p2);
	else if (dy == 0)
		draw_horizontal_line(fdf, p1, p2);
	else if (dx == dy)
		draw_line_slope_abs(fdf, p1, p2);
	else if (dx > dy)
		draw_line_move_x(fdf, p1, p2);
	else if (dx < dy)
		draw_line_move_y(fdf, p1, p2);
}

static void	draw_vertical_line(t_fdf *fdf, t_point p1, t_point p2)
{
	t_point	syp;
	t_point	byp;

	if (p1.ry2d < p2.ry2d)
	{
		syp = p1;
		byp = p2;
	}
	else
	{
		syp = p2;
		byp = p1;
	}
	draw_vertical_line2(fdf, syp, byp);
}

static void	draw_vertical_line2(t_fdf *fdf, t_point syp, t_point byp)
{
	t_color	*color_box;
	int		np;

	np = byp.ry2d - syp.ry2d - 1;
	if (fdf->color_mode == 0)
		color_box = calc_color(syp.color, byp.color, np);
	else
		color_box = calc_color(fdf->contour_color[(int)(syp.z
					- fdf->map_ptr->smallest_z)], fdf->contour_color[(int)(byp.z
					- fdf->map_ptr->smallest_z)], np);
	if (color_box == T_NULL)
	{
		fdf_free(fdf);
		err_msg("malloc error!", 1, FALSE);
	}
	draw_vertical_line3(fdf, syp, byp, color_box);
}

static void	draw_vertical_line3(t_fdf *fdf,
		t_point syp, t_point byp, t_color *color_box)
{
	t_point	*point_box;
	int		idx;
	int		np;

	np = byp.ry2d - syp.ry2d - 1;
	point_box = (t_point *)malloc(sizeof(t_point) * np);
	if (point_box == T_NULL)
	{
		fdf_free(fdf);
		free(color_box);
		err_msg("malloc error!", 1, FALSE);
	}
	idx = -1;
	while (++idx < np)
	{
		point_box[idx].ry2d = syp.ry2d + 1 + idx;
		point_box[idx].rx2d = syp.rx2d;
	}
	mlx_pixels_put_at_mem(fdf, point_box, color_box, np);
	free(color_box);
	free(point_box);
}
